#include "pch.h"
#include "paplease/analysis/required_docs_tracker.h"

namespace paplease {
    namespace analysis {
        std::string DocLookup::ToString() const noexcept
        {
            return std::format("{}:{}",
                               magic_enum::enum_name(documentType),
                               magic_enum::enum_name(passportType));
        }


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

        void RequiredDocsTracker::AddRequiredDocument(DocLookup docLookup)
        {
            m_requiredDocs.Set(docLookup, RequiredDocStatus{docLookup});
        }

        void RequiredDocsTracker::RemoveRequiredDocument(DocLookup docLookup)
        {
            m_requiredDocs.Remove(docLookup);
        }

        // Returns true if document is required, up until it has been fully analyzed, whence it will return false
        bool RequiredDocsTracker::Requires(documents::AppearanceType appearanceType) const noexcept
        {
            DocLookup lookupType{ appearanceType };
            //if (lookupType.passportType == documents::PassportType::Invalid && lookupType.documentType != documents::DocType::Passport)
            if (lookupType.documentType != documents::DocType::Passport)
            {  // lookup type is not passport (passport is special case)
                return m_requiredDocs.Contains(lookupType);
            }

            DocLookup passportLookupType = this->TryGetPassportLookupType();
            if (passportLookupType.documentType != documents::DocType::Passport)
            {
                return false;
            }

            if (passportLookupType.passportType == documents::PassportType::Invalid)
            {  // matches passport of any type
                return true;
            }

            return passportLookupType.passportType == lookupType.passportType;
        }

        void RequiredDocsTracker::SetExists(documents::AppearanceType appearanceType)
        {
            DocLookup lookupType = this->GetDocumentLookupType(appearanceType);
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
            DocLookup lookupType = this->GetDocumentLookupType(appearanceType);
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

            // This following code is a thought
            this->RemoveRequiredDocument(lookupType);
        }

        DocLookup RequiredDocsTracker::GetDocumentLookupType(documents::AppearanceType appearanceType) const noexcept
        {
            DocLookup lookupType{ appearanceType };
            if (lookupType.passportType == documents::PassportType::Invalid && lookupType.documentType != documents::DocType::Passport)
            {
                return lookupType;
            }
            
            DocLookup passportQualifier = this->TryGetPassportLookupType();
            return passportQualifier;
        }

        DocLookup RequiredDocsTracker::TryGetPassportLookupType() const noexcept
        {
            for (auto& kv : m_requiredDocs)
            {
                if (kv->lookupType.documentType == documents::DocType::Passport)
                {
                    return kv->lookupType;
                }
            }

            return DocLookup{ documents::AppearanceType::Invalid };
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
