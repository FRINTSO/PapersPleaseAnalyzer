#include "pch.h"
#include "paplease/analysis/doc_tracker.h"

#include "paplease/scannable/booth.h"
#include "paplease/documents/doc_type.h"

namespace paplease {
    namespace analysis {

		void DocTracker::TrackDocument(const documents::DocView& docView)
		{
			
		}

		// Refresh all trackers
//		void DocTracker::RefreshTracking(const scannable::ScanContext& scanContext)
//		{
//			UpdateRequiredDocuments();
//			m_visibleDocuments.Clear();
//			RegisterScannedDocuments(scanContext.boothData.scannedDocuments);
//			RegisterScannedDocuments(scanContext.inspectionData.scannedDocuments);
//
//#if DEBUG_LOG_DOCUMENT_TRACKING
//			auto& documents = m_store.GetStoredCollection<documents::DocView>();
//			for (const auto& entry : documents)
//			{
//				DocType documentType = ToDocType(entry.value.appearanceType);
//				if (!IsApplicantDocument(documentType))
//				{
//					continue;
//				}
//
//				if (m_visibleDocuments.Contains(entry.key))
//				{ // Visible
//					LOG("Visible: '{}'", magic_enum::enum_name(documentType));
//				}
//				else
//				{ // Not visible
//					LOG("Invisible: '{}'", magic_enum::enum_name(documentType));
//				}
//			}
//#endif
//		}
//
//		void DocTracker::RegisterScannedDocuments(const scannable::DocViewCollection& scannedDocuments)
//		{
//			for (documents::DocView scannedDocView : scannedDocuments)
//			{
//				RegisterSingleScannedDocument(std::move(scannedDocView));
//			}
//		}
//
//		void DocTracker::RegisterSingleScannedDocument(documents::DocView&& scannedDocView)
//		{
//			const auto documentType = ToDocType(scannedDocView.appearanceType);
//
//			// Mark as scanned and visible
//			m_store.GetAnalysisStatus(documentType).wasScanned = true;
//			m_visibleDocuments.Insert(documentType);
//
//			if (m_store.Contains<documents::DocView>(documentType))
//			{
//				// Update existing document if it moved
//				documents::DocView& storedDocView = m_store.GetStored<documents::DocView>(documentType);
//				const bool viewAreaChanged = storedDocView.viewArea != scannedDocView.viewArea;
//				const bool moved = cv::Rect(storedDocView.boundingBox) != cv::Rect(scannedDocView.boundingBox);
//
//				if (!viewAreaChanged && !moved)
//					return;
//
//#if DEBUG_LOG_DOCUMENT_TRACKING
//				if (viewAreaChanged)
//				{
//					LOG("Document '{}' changed from {} to {}",
//						magic_enum::enum_name(scannedDocView.appearanceType),
//						magic_enum::enum_name(storedDocView.viewArea),
//						magic_enum::enum_name(scannedDocView.viewArea));
//				}
//				else if (moved)
//				{
//					LOG("Document '{}' moved",
//						magic_enum::enum_name(scannedDocView.appearanceType));
//				}
//#endif
//				storedDocView.boundingBox = scannedDocView.boundingBox;
//				storedDocView.viewArea = scannedDocView.viewArea;
//				return;
//			}
//
//			// Handle new document
//#if DEBUG_LOG_DOCUMENT_TRACKING
//			LOG("New document '{}' in {}",
//				magic_enum::enum_name(scannedDocView.appearanceType),
//				magic_enum::enum_name(scannedDocView.viewArea));
//#endif
//
//			LOG("[Document {}] has been detected!",
//				magic_enum::enum_name(documentType));
//
//			if (documentType == documents::DocType::Passport)
//			{
//				const auto country = data::LocationBank::GetCountryFromPassportType(documents::ToPassportType(scannedDocView.appearanceType));
//				m_context.SetEntrantCountry(country);
//			}
//
//			m_store.StoreDocumentView(std::move(scannedDocView));
//		}
//
//
//		// Call after tracking call, to fetch all documents
//		DocTracker::DocTypeSet DocTracker::GetVisibleDocuments() const
//		{
//			return m_visibleDocuments;
//		}
//
//		DocTracker::DocTypeSet DocTracker::GetUpdatedDocuments() const
//		{
//			throw "Unimplemented";
//		}
//
//
//		// Talking to user and informing
//		void DocTracker::ReportMissingDocuments() const noexcept
//		{
//			// Required documents not in m_store
//			for (auto& [documentType, passportType] : m_store.GetRequiredTypes())
//			{
//				auto& status = m_store.GetAnalysisStatus(documentType);
//				if (status.wasScanned)
//					continue;
//
//				if (!status.isRequiredReported)
//				{
//					if (passportType == documents::PassportType::Invalid)
//					{
//						LOG_WARN("[Document {}] is required.",
//								 magic_enum::enum_name(documentType));
//					}
//					else
//					{
//						const auto appearanceType = ToAppearanceType(documentType, passportType);
//						LOG_WARN("[Document {}] is required.",
//								 magic_enum::enum_name(appearanceType));
//					}
//
//					status.isRequiredReported = true;
//				}
//			}
//		}
//
//		void DocTracker::UpdateRequiredDocuments()
//		{
//			for (const auto& trackedRule : m_context.GetApplicableRules())
//			{
//				const auto rule = trackedRule->rule;
//				switch (rule.GetRule())
//				{
//					case data::ERule::RequireAccessPermitFromForeigners:
//					case data::ERule::RequireArstotzkanPassportFromEntrant:
//					case data::ERule::RequireDiplomaticAuthorizationFromDiplomats:
//					case data::ERule::RequireEntryPermitFromForeigners:
//					case data::ERule::RequireEntryTicketFromForeigners:
//					case data::ERule::RequireGrantFromAsylumSeekers:
//					case data::ERule::RequireIdentityCardFromCitizens:
//					case data::ERule::RequireIdentitySupplementFromForeigners:
//					case data::ERule::RequirePassportFromEntrant:
//					case data::ERule::RequirePolioVaccinationFromEntrant:
//					case data::ERule::RequireWorkPassFromWorkers:
//					{
//						const auto requiredType = data::ERuleSubjectToDocType(rule.GetDescriptor().subject);
//						m_store.SetRequiredType(requiredType);
//						break;
//					}
//					default:
//						continue;
//				}
//			}
//		}
//
//#pragma endregion

    }  // namespace analysis
}  // namespace paplease
