#include "pch.h"
#include "paplease/analysis/doc_tracker.h"

#include <magic_enum/magic_enum.hpp>

#include "paplease/scannable/booth.h"
#include "paplease/documents/doc_appearance.h"
#include "paplease/documents/doc_type.h"
#include "paplease/documents/doc_layout.h"
#include "paplease/common/image_process.h"
#include "paplease/common/shape.h"
#include "paplease/utils/enum_range.h"

#include "test/documents/test_hsv.h"

namespace paplease {
    namespace analysis {

		using namespace documents;

#pragma region DocTracker

		DocTracker::DocTracker()
			: m_documentPositions{ TrackedDocument{ DocType::Invalid, Rectangle{ }} }, m_documentsInBoothView{DocType::Invalid}
		{
		}

        static cv::Mat PreprocessBoothView(const GameView& view)
        {
            // Minimized documents
            static constinit HSVConfig config1{ 36, 36, 0 ,255, 0, 255 };
            auto binary1 = ToHSVBinary(view.booth(), config1);
            static constinit HSVConfig config2{ 11, 24, 77 ,147, 66, 139 };
            auto binary2 = ToHSVBinary(view.booth(), config2);
            static auto boothDocArea = LoadImageFile("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\booth\\booth_doc_area.png");
            cv::Mat binaryMask;
            cv::bitwise_or(binary1, binary2, binaryMask, boothDocArea);

            cv::Mat result;
            cv::bitwise_and(view.booth(), view.booth(), result, binaryMask ^ 255);
                
            cv::Mat result2;
            cv::bitwise_and(result, result, result2, boothDocArea);
            return result2;
        }

        static cv::Mat PreprocessInspectionView(const GameView& view)
        {
            static constinit HSVConfig config1{0, 179, 0, 255, 51, 255};
            // test::documents::find_hsv(view.inspection, config1);
            auto binary1 = ToHSVBinary(view.inspection(), config1);

            static auto inspectionViewFilter = LoadImageFile("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\booth\\inspection_view_filter.png") ^ 255;

            cv::Mat binaryMask = binary1 & inspectionViewFilter;

            cv::imshow("Binmask", binaryMask);

            cv::Mat result;
            cv::bitwise_and(view.inspection(), view.inspection(), result, binaryMask);
            return result;
        }

        static inline auto GetAppearanceTypes()
        {
            using namespace documents;
			std::array<const DocAppearance*, static_cast<size_t>(AppearanceType::Count)> appearanceTypes{};

            for (auto appearanceType : utils::enum_range(AppearanceType::AccessPermit, AppearanceType::Passport_UnitedFederation))
            {
                appearanceTypes[static_cast<size_t>(appearanceType)] = &DocAppearance::GetRef(appearanceType);
            }

            return appearanceTypes;
        }

		static inline Rectangle FindDocumentBoundingBox(const cv::Mat& inspection, const documents::DocAppearance& appearance)
		{
			int left = INT_MAX;
			int top = INT_MAX;
			int right = 0;
			int bottom = 0;
			bool hasFoundTopLeft = false;

#if MATCH_ONE_COLOR
			const auto color = RGB_VAL(appearance.GetColors()[0]);
#else
			const RgbColor* colors = appearance.GetColors();
			const size_t size = appearance.GetColorCount();
#endif

			int maxScanY = inspection.rows;
			int minScanX = 0;
			int maxScanX = inspection.cols;

			for (int y = 0; y < maxScanY; y++)
			{
				const BgrColor* bgr = inspection.ptr<BgrColor>(y);

				int min_x = INT_MAX;
				int max_x = 0;
				bool colorMatchedInRow = false;

				for (int x = minScanX; x < maxScanX; x++)
				{
					// CHECK IF BGR_VAL(bgr[x]) is in colors
#if MATCH_ONE_COLOR
					if (color != BGR_VAL(bgr[x])) continue;
#else
					bool colorMatch = false;
					for (size_t i = 0; i < size; i++)
					{
						if (RGB_VAL(colors[i]) == BGR_VAL(bgr[x]))
						{
							colorMatch = true;
							break;
						}
					}
					if (!colorMatch) continue;
#endif
#if CHEEKY_OPTIMIZATION
					// cheeky optimization
					if (!hasFoundTopLeft)
					{
						hasFoundTopLeft = !hasFoundTopLeft;

						int width = appearance.GetWidth();
						int height = appearance.GetHeight();

						if (y + height - 1 < inspection.rows && x + width - 1 < inspection.cols)
						{
							// bottom_right
							const auto& bottomRightColor = *inspection.ptr<BgrColor>(y + height - 1, x + width - 1);
#if MATCH_ONE_COLOR
							if (color == BGR_VAL(bgr[x]))
								return Rectangle{ x,y,width,height };
#else
							for (size_t i = 0; i < size; i++)
							{
								if (RGB_VAL(colors[i]) == BGR_VAL(bottomRightColor))
								{
									return Rectangle{ x, y, width, height };
								}
							}
#endif
						}
					}
#endif

					colorMatchedInRow = true;

					min_x = std::min(min_x, x);
					max_x = std::max(max_x, x);
				}

				if (colorMatchedInRow)
				{
					if (y < top)
					{
						top = y;
#if EFFECTIVE_SCANNING_OPTIMIZATION
						int maxHeight = top + appearance.GetHeight();
						maxScanY = (maxHeight > inspection.rows) ? inspection.rows : maxHeight;
#endif
					}

					bottom = std::max(bottom, y);

					if (min_x < left)
					{
						left = min_x;
#if EFFECTIVE_SCANNING_OPTIMIZATION
						int maxWidth = left + appearance.GetWidth();
						maxScanX = (maxWidth > inspection.cols) ? inspection.cols : maxWidth;
#endif
					}
					if (max_x > right)
					{
						right = max_x;
#if EFFECTIVE_SCANNING_OPTIMIZATION
						int minWidth = right - appearance.GetWidth() + 1;
						minScanX = (minWidth < 0) ? 0 : minWidth;
#endif
					}
				}
			}

			// Nothing was found
			if (left == INT_MAX && top == INT_MAX && !right && !bottom) return Rectangle{};

			//cv::imshow("DBG_DOC_BOX", inspection(cv::Rect(left, top, right - left + 1, bottom - top + 1)));
			//cv::waitKey();

			return Rectangle{ left, top, right - left + 1, bottom - top + 1 };
		}

        void DocTracker::TrackDocsInBoothView(const GameView& view)
        {
            auto boothView = PreprocessBoothView(view);  // TODO: Remove unscannable part
                
			for (auto docAppearance : GetAppearanceTypes())
			{
				auto boundingBox = FindDocumentBoundingBox(boothView, *docAppearance);
				if (boundingBox.Empty()) continue;

				cv::rectangle(boothView, boundingBox, cv::Scalar(127));
					
				auto docType = documents::ToDocType(docAppearance->GetType());
				if (!m_documentPositions.Contains(docType))
				{
 					m_documentPositions.Set(docType, { docType, boundingBox, ViewArea::BoothView });
				}
				else
				{
					m_documentPositions.Get(docType).box = boundingBox;
				}

			}
			cv::imshow("Booth View", boothView);
			cv::waitKey(30);
        }

        void DocTracker::TrackDocsInInspectionView(const GameView& view)
        {
            // Full-sized documents
            auto inspectionView = PreprocessInspectionView(view);
            cv::imshow("Inspection View", inspectionView);
            cv::waitKey(30);
        }


        void DocTracker::Update(const GameView& gameView)
        {
            // Doc can either be in booth view or in inspection view

            // Track docs in booth
            TrackDocsInBoothView(gameView);

            // Track docs in inspection
            // TrackDocsInInspectionView(gameView);


        }

		void DocTracker::OnNewDate()
		{
			m_documentPositions.Clear();
			m_documentsInBoothView.Clear();
		}
		void DocTracker::OnNewApplicant()
		{
			m_documentPositions.Clear();
			m_documentsInBoothView.Clear();
		}

#pragma endregion

#pragma region DocTrackerV2

		void DocTrackerV2::RegisterScannedDocuments(const scannable::DocViewCollection& scannedDocuments)
		{
			core::FixedHashSet<documents::DocType, static_cast<size_t>(documents::DocType::Count)> seenDocuments{};

			for (const auto& docView : scannedDocuments)
			{
				auto documentType = ToDocType(docView.appearanceType);
				seenDocuments.Insert(documentType);
				if (m_documents.Contains(documentType))
				{  // Already stored document - Update position & view area
					auto& storedDocView = m_documents.Get(documentType);

					if (storedDocView.viewArea != docView.viewArea)
					{  // Document was moved from one scan area to the other
						LOG(
							"Document '{}' changed from {} to {}",
							magic_enum::enum_name<documents::AppearanceType>(docView.appearanceType),
							magic_enum::enum_name<ViewArea>(storedDocView.viewArea),
							magic_enum::enum_name<ViewArea>(docView.viewArea)
						);
					}
					else if (cv::Rect(storedDocView.boundingBox) != cv::Rect(docView.boundingBox))
					{
						LOG(
							"Document '{}' moved",
							magic_enum::enum_name<documents::AppearanceType>(docView.appearanceType),
							magic_enum::enum_name<ViewArea>(storedDocView.viewArea),
							magic_enum::enum_name<ViewArea>(docView.viewArea)
						);
					}
					else
					{
						continue;
					}

					storedDocView.boundingBox = docView.boundingBox;
					storedDocView.viewArea = docView.viewArea;
				}
				else
				{  // New document
					LOG(
						"New document '{}' in {}",
						magic_enum::enum_name<documents::AppearanceType>(docView.appearanceType),
						magic_enum::enum_name<ViewArea>(docView.viewArea)
					);
					m_documents.Set(documentType, docView);
				}
			}

			for (auto& doc : m_documents)
			{
				if (doc.appearanceType == AppearanceType::Invalid || doc.viewArea == ViewArea::BoothView) 
				{
					continue;
				}
				const auto docType = ToDocType(doc.appearanceType);
				if (!seenDocuments.Contains(docType))
				{
					m_documentsNotVisible.Insert(docType);
				}
			}
		}

		void DocTrackerV2::RefreshTracking(const scannable::ScanContext& scanContext)
		{
			RegisterScannedDocuments(scanContext.boothData.scannedDocuments);
			m_documentsNotVisible.Clear();
			RegisterScannedDocuments(scanContext.inspectionData.scannedDocuments);

			for (auto docType : m_documentsNotVisible)
			{
				LOG(
					"Can't see document '{}'",
					magic_enum::enum_name<documents::DocType>(docType)
				);
			}
		}

#pragma endregion


    }  // namespace analysis
}  // namespace paplease
