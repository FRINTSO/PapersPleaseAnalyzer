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
			: m_documentPositions{ TrackedDocument{ DocType::Invalid, Rectangle{ }} }, m_documentsInBoothView{}
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

#if EXPERIMENTAL_MATCH_ONE_COLOR
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
#if EXPERIMENTAL_MATCH_ONE_COLOR
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
#if OPTIMIZE_CHEEKY
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
#if EXPERIMENTAL_MATCH_ONE_COLOR
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
#if OPTIMIZE_EFFECTIVE_SCANNING
						int maxHeight = top + appearance.GetHeight();
						maxScanY = (maxHeight > inspection.rows) ? inspection.rows : maxHeight;
#endif
					}

					bottom = std::max(bottom, y);

					if (min_x < left)
					{
						left = min_x;
#if OPTIMIZE_EFFECTIVE_SCANNING
						int maxWidth = left + appearance.GetWidth();
						maxScanX = (maxWidth > inspection.cols) ? inspection.cols : maxWidth;
#endif
					}
					if (max_x > right)
					{
						right = max_x;
#if OPTIMIZE_EFFECTIVE_SCANNING
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
			for (const auto& docView : scannedDocuments)
			{
				auto documentType = ToDocType(docView.appearanceType);
				m_visibleDocuments.Insert(documentType);
				if (m_documents.Contains(documentType))
				{  // Already stored document - Update position & view area
					DocView storedDocView;
					m_documents.Get(documentType, storedDocView);

					if (storedDocView.viewArea != docView.viewArea)
					{  // Document was moved from one scan area to the other
#if DEBUG_LOG_DOCUMENT_TRACKING
						LOG(
							"Document '{}' changed from {} to {}",
							magic_enum::enum_name<documents::AppearanceType>(docView.appearanceType),
							magic_enum::enum_name<ViewArea>(storedDocView.viewArea),
							magic_enum::enum_name<ViewArea>(docView.viewArea)
						);
#endif
					}
					else if (cv::Rect(storedDocView.boundingBox) != cv::Rect(docView.boundingBox))
					{
#if DEBUG_LOG_DOCUMENT_TRACKING
						LOG(
							"Document '{}' moved",
							magic_enum::enum_name<documents::AppearanceType>(docView.appearanceType),
							magic_enum::enum_name<ViewArea>(storedDocView.viewArea),
							magic_enum::enum_name<ViewArea>(docView.viewArea)
						);
#endif
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
#if DEBUG_LOG_DOCUMENT_TRACKING
					LOG(
						"New document '{}' in {}",
						magic_enum::enum_name<documents::AppearanceType>(docView.appearanceType),
						magic_enum::enum_name<ViewArea>(docView.viewArea)
					);
#endif
					m_documents.Set(documentType, docView);
				}
			}

		}

		void DocTrackerV2::RefreshTracking(const scannable::ScanContext& scanContext)
		{
			m_visibleDocuments.Clear();
			RegisterScannedDocuments(scanContext.boothData.scannedDocuments);
			RegisterScannedDocuments(scanContext.inspectionData.scannedDocuments);

#if DEBUG_LOG_DOCUMENT_TRACKING
			for (const auto& entry : m_documents)
			{
				DocType documentType = ToDocType(entry.value.appearanceType);
				if (!IsApplicantDocument(documentType))
				{
					continue;
				}

				if (m_visibleDocuments.Contains(entry.key))
				{ // Visible
					LOG("Visible: '{}'", magic_enum::enum_name<documents::DocType>(documentType));
				}
				else
				{ // Not visible
					LOG("Invisible: '{}'", magic_enum::enum_name<documents::DocType>(documentType));
				}
			}
#endif
		}

		bool DocTrackerV2::Contains(documents::DocType documentType) const noexcept
		{
			return m_documents.Contains(documentType);
		}

		bool DocTrackerV2::Contains(documents::DocType documentType, documents::PassportType passportType) const noexcept
		{
			documents::DocView docView;
			if (m_documents.Get(documentType, docView))
			{
				return ToAppearanceType(documentType, passportType) == docView.appearanceType;
			}
			return false;
		}

		bool DocTrackerV2::IsVisible(documents::DocType documentType) const noexcept
		{
			return m_visibleDocuments.Contains(documentType);
		}

		bool DocTrackerV2::IsVisible(documents::DocType documentType, ViewArea viewArea) const noexcept
		{
			if (!m_visibleDocuments.Contains(documentType))
			{
				return false;
			}

			const documents::DocView& docView = m_documents[documentType];
			return docView.viewArea == viewArea;
		}

		bool DocTrackerV2::ExtractDoc(const GameView& gameView, documents::DocType documentType, documents::Doc& outDocument) const noexcept
		{
			if (!m_visibleDocuments.Contains(documentType))
			{
				return false;
			}
			const documents::DocView& docView = m_documents[documentType];
			outDocument = docView.ToDocument(gameView);
			return true;
		}

		void DocTrackerV2::AddRequiredDocument(std::pair<documents::DocType, documents::PassportType> docType)
		{
			if (m_requiredDocuments.Contains(docType))
			{
				return;
			}

#if DEBUG_LOG_REQUIRED_DOCUMENTS
			if (docType.second != documents::PassportType::Invalid)
			{
				auto appearanceType = ToAppearanceType(docType.first, docType.second);
				LOG("Required document: {}", magic_enum::enum_name<documents::AppearanceType>(appearanceType));
			}
			else
			{
				LOG("Required document: {}", magic_enum::enum_name<documents::DocType>(docType.first));
			}
#endif
			m_requiredDocuments.Set(docType, true);
		}

#pragma endregion


    }  // namespace analysis
}  // namespace paplease
