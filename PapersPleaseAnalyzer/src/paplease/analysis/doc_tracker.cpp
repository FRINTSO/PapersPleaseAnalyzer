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

		// Refresh all trackers
		void DocTrackerV2::RefreshTracking(const scannable::ScanContext& scanContext)
		{
			UpdateRequiredDocuments();
			m_visibleDocuments.Clear();
			RegisterScannedDocuments(scanContext.boothData.scannedDocuments);
			RegisterScannedDocuments(scanContext.inspectionData.scannedDocuments);

#if DEBUG_LOG_DOCUMENT_TRACKING
			auto& documents = m_store.GetStoredCollection<documents::DocView>();
			for (const auto& entry : documents)
			{
				DocType documentType = ToDocType(entry.value.appearanceType);
				if (!IsApplicantDocument(documentType))
				{
					continue;
				}

				if (m_visibleDocuments.Contains(entry.key))
				{ // Visible
					LOG("Visible: '{}'", magic_enum::enum_name(documentType));
				}
				else
				{ // Not visible
					LOG("Invisible: '{}'", magic_enum::enum_name(documentType));
				}
			}
#endif
		}

		void DocTrackerV2::RegisterScannedDocuments(const scannable::DocViewCollection& scannedDocuments)
		{
			for (documents::DocView scannedDocView : scannedDocuments)
			{
				RegisterSingleScannedDocument(std::move(scannedDocView));
			}
		}

		void DocTrackerV2::RegisterSingleScannedDocument(documents::DocView&& scannedDocView)
		{
			const auto documentType = ToDocType(scannedDocView.appearanceType);

			// Mark as scanned and visible
			m_store.GetAnalysisStatus(documentType).wasScanned = true;
			m_visibleDocuments.Insert(documentType);

			if (m_store.Contains<documents::DocView>(documentType))
			{
				// Update existing document if it moved
				DocView& storedDocView = m_store.GetStored<documents::DocView>(documentType);
				const bool viewAreaChanged = storedDocView.viewArea != scannedDocView.viewArea;
				const bool moved = cv::Rect(storedDocView.boundingBox) != cv::Rect(scannedDocView.boundingBox);

				if (!viewAreaChanged && !moved)
					return;

#if DEBUG_LOG_DOCUMENT_TRACKING
				if (viewAreaChanged)
				{
					LOG("Document '{}' changed from {} to {}",
						magic_enum::enum_name(scannedDocView.appearanceType),
						magic_enum::enum_name(storedDocView.viewArea),
						magic_enum::enum_name(scannedDocView.viewArea));
				}
				else if (moved)
				{
					LOG("Document '{}' moved",
						magic_enum::enum_name(scannedDocView.appearanceType));
				}
#endif
				storedDocView.boundingBox = scannedDocView.boundingBox;
				storedDocView.viewArea = scannedDocView.viewArea;
				return;
			}

			// Handle new document
#if DEBUG_LOG_DOCUMENT_TRACKING
			LOG("New document '{}' in {}",
				magic_enum::enum_name(scannedDocView.appearanceType),
				magic_enum::enum_name(scannedDocView.viewArea));
#endif

			LOG("[Document {}] has been detected!",
				magic_enum::enum_name(documentType));

			if (documentType == documents::DocType::Passport)
			{
				const auto country = data::LocationBank::GetCountryFromPassportType(documents::ToPassportType(scannedDocView.appearanceType));
				m_context.SetEntrantCountry(country);
			}

			m_store.StoreDocumentView(std::move(scannedDocView));
		}


		// Call after tracking call, to fetch all documents
		DocTrackerV2::DocTypeSet DocTrackerV2::GetVisibleDocuments() const
		{
			return m_visibleDocuments;
		}

		DocTrackerV2::DocTypeSet DocTrackerV2::GetUpdatedDocuments() const
		{
			throw "Unimplemented";
		}


		// Talking to user and informing
		void DocTrackerV2::ReportMissingDocuments() const noexcept
		{
			// Required documents not in m_store
			for (auto& [documentType, passportType] : m_store.GetRequiredTypes())
			{
				auto& status = m_store.GetAnalysisStatus(documentType);
				if (status.wasScanned)
					continue;

				if (!status.isRequiredReported)
				{
					if (passportType == documents::PassportType::Invalid)
					{
						LOG_WARN("[Document {}] is required.",
								 magic_enum::enum_name(documentType));
					}
					else
					{
						const auto appearanceType = ToAppearanceType(documentType, passportType);
						LOG_WARN("[Document {}] is required.",
								 magic_enum::enum_name(appearanceType));
					}

					status.isRequiredReported = true;
				}
			}
		}


		void DocTrackerV2::AddRequiredDocument(std::pair<documents::DocType, documents::PassportType> docType)
		{
//			if (m_requiredDocuments.Contains(docType))
//			{
//				return;
//			}
//
//#if DEBUG_LOG_REQUIRED_DOCUMENTS
//			if (docType.second != documents::PassportType::Invalid)
//			{
//				auto appearanceType = ToAppearanceType(docType.first, docType.second);
//				LOG("Required document: {}", magic_enum::enum_name(appearanceType));
//			}
//			else
//			{
//				LOG("Required document: {}", magic_enum::enum_name(docType.first));
//			}
//#endif
//			m_requiredDocuments.Set(docType, true);
		}

		void DocTrackerV2::UpdateRequiredDocuments()
		{
			for (const auto& trackedRule : m_context.GetApplicableRules())
			{
				const auto rule = trackedRule->rule;
				switch (rule.GetRule())
				{
					case data::ERule::RequireAccessPermitFromForeigners:
					case data::ERule::RequireArstotzkanPassportFromEntrant:
					case data::ERule::RequireDiplomaticAuthorizationFromDiplomats:
					case data::ERule::RequireEntryPermitFromForeigners:
					case data::ERule::RequireEntryTicketFromForeigners:
					case data::ERule::RequireGrantFromAsylumSeekers:
					case data::ERule::RequireIdentityCardFromCitizens:
					case data::ERule::RequireIdentitySupplementFromForeigners:
					case data::ERule::RequirePassportFromEntrant:
					case data::ERule::RequirePolioVaccinationFromEntrant:
					case data::ERule::RequireWorkPassFromWorkers:
					{
						const auto requiredType = data::ERuleSubjectToDocType(rule.GetDescriptor().subject);
						m_store.SetRequiredType(requiredType);
						break;
					}
					default:
						continue;
				}
			}
		}

#pragma endregion

    }  // namespace analysis
}  // namespace paplease
