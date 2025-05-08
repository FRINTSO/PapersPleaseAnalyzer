#include "pch.h"
#include "paplease/analysis/required_docs_tracker.h"

namespace paplease {
    namespace analysis {

        core::FixedArray<RequiredDocStatus, RequiredDocsTracker::DocTypeSize> RequiredDocsTracker::GetNonAnalyzedRequiredDocuments() const
        {
            core::FixedArray<RequiredDocStatus, RequiredDocsTracker::DocTypeSize> remainingDocs{};

            for (auto& kv : m_requiredDocs)
            {
                if (!kv->analyzed)
                {
                    remainingDocs.Add(kv.value);
                }
            }
            return remainingDocs;
        }

        void RequiredDocsTracker::AddRequiredDocument(documents::DocLookup docLookup)
        {
            // Don't add if we already have document as requirement
            if (!m_requiredDocs.Contains(docLookup))
            {
                m_requiredDocs.Set(docLookup, RequiredDocStatus{docLookup});
                m_requiredCount++;
            }
        }

        void RequiredDocsTracker::RemoveRequiredDocument(documents::DocLookup docLookup)
        {
            m_requiredDocs.Remove(docLookup);
        }

        // Returns true if document is required, up until it has been fully analyzed, whence it will return false
        bool RequiredDocsTracker::Requires(documents::AppearanceType appearanceType) const noexcept
        {
            documents::DocLookup lookupType{ appearanceType };
            //if (lookupType.passportType == documents::PassportType::Invalid && lookupType.documentType != documents::DocType::Passport)
            if (lookupType.documentType != documents::DocType::Passport)
            {  // lookup type is not passport (passport is special case)
                const RequiredDocStatus* outDocStatus = nullptr;
                if (m_requiredDocs.Get(lookupType, outDocStatus))
                {
                    return !outDocStatus->analyzed;
                }
                return false;
            }
            documents::DocLookup passportLookupType = this->TryGetPassportLookupType();
            if (lookupType.AppliesTo(passportLookupType))
            {
                const RequiredDocStatus* outDocStatus = nullptr;
                if (m_requiredDocs.Get(passportLookupType, outDocStatus))
                {
                    return !outDocStatus->analyzed;
                }
            }
            return false;
        }

        void RequiredDocsTracker::SetExists(documents::AppearanceType appearanceType)
        {
            documents::DocLookup lookupType = this->GetDocumentLookupType(appearanceType);
            if (!lookupType.IsValid())
            {
                return;
            }

            if (!m_requiredDocs.Contains(lookupType))
            {
                LOG_WARN("DocumentStatuses doesn't contain lookup type: {}", lookupType.ToString());
            }

            auto& docStatus = m_requiredDocs[lookupType];
            docStatus.exists = true;
        }
        void RequiredDocsTracker::SetAnalyzed(documents::AppearanceType appearanceType)
        {
            documents::DocLookup lookupType = this->GetDocumentLookupType(appearanceType);
            if (!lookupType.IsValid())
            {
                return;
            }

            if (!m_requiredDocs.Contains(lookupType))
            {
                LOG_WARN("DocumentStatuses doesn't contain lookup type: {}", lookupType.ToString());
            }

            auto& docStatus = m_requiredDocs[lookupType];
            docStatus.analyzed = true;
            m_requiredCount--;
        }

        documents::DocLookup RequiredDocsTracker::GetDocumentLookupType(documents::AppearanceType appearanceType) const noexcept
        {
            documents::DocLookup lookupType{ appearanceType };
            if (lookupType.passportType == documents::PassportType::Invalid && lookupType.documentType != documents::DocType::Passport)
            {
                return lookupType;
            }
            
            documents::DocLookup passportQualifier = this->TryGetPassportLookupType();
            return passportQualifier;
        }

        documents::DocLookup RequiredDocsTracker::TryGetPassportLookupType() const noexcept
        {
            for (auto& kv : m_requiredDocs)
            {
                if (kv->lookupType.documentType == documents::DocType::Passport)
                {
                    return kv->lookupType;
                }
            }

            return documents::DocLookup{ documents::AppearanceType::Invalid };
        }

        void RequiredDocsTracker::WarnMissingRequiredDocuments()
        {
            for (auto& kv : m_requiredDocs)
            {
                RequiredDocStatus& docStatus = kv.value;
                if (!docStatus.exists && !docStatus.hasWarnedNotSpotted)
                {
                    LOG_WARN("[Document '{}:{}' has not been spotted]",
                             magic_enum::enum_name(docStatus.lookupType.documentType),
                             magic_enum::enum_name(docStatus.lookupType.passportType));
                    docStatus.hasWarnedNotSpotted = true;
                }
                else if (docStatus.exists && !docStatus.analyzed && !docStatus.hasWarnedNotAnalyzed)
                {
                    LOG_WARN("[Document '{}:{}' has not been analyzed]",
                             magic_enum::enum_name(docStatus.lookupType.documentType),
                             magic_enum::enum_name(docStatus.lookupType.passportType));
                    docStatus.hasWarnedNotAnalyzed = true;
                }
            }
        }

    }  // namespace analysis
}  // namespace paplease
