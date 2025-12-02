#pragma once
#include "paplease/core/fixed.h"
#include "paplease/documents/doc_lookup.h"
#include "paplease/documents/doc_type.h"

namespace paplease {
    namespace analysis {

        struct RequiredDocStatus
        {
            RequiredDocStatus()
                : lookupType(documents::AppearanceType::Invalid),
                exists(false),
                analyzed(false)
            {}
            RequiredDocStatus(documents::DocLookup lookupType)
                : lookupType(lookupType),
                exists(documents::IsEntrantDocument(lookupType.documentType) ? false : true),
                // spotted(false),
                analyzed(false)
            {}

            documents::DocLookup lookupType;
            bool exists;  // if it exists, then we can prompt the user to analyze it. Can't do that if it doesn't exist. Rulebook, Transcript, Bulletin always exists
            bool analyzed;

            // logging fields
            bool hasWarnedNotSpotted = false;
            bool hasWarnedNotAnalyzed = false;
        };

        class RequiredDocsTracker
        {
            static constexpr size_t DocTypeSize = static_cast<size_t>(documents::DocType::Count);
            using DocLookupTypes = core::FixedHashSet<documents::DocLookup, DocTypeSize>;
        public:
            RequiredDocsTracker() noexcept
                : m_requiredDocs()
            {
                this->AddRequiredDocument({ documents::DocType::RuleBook });
            }

            void AddRequiredDocument(documents::DocLookup docLookup);
            bool Requires(documents::AppearanceType appearanceType) const noexcept;

            void WarnMissingRequiredDocuments();

            // Get documents that are required, but that haven't been analyzed
            core::FixedArray<RequiredDocStatus, DocTypeSize> GetNonAnalyzedRequiredDocuments() const;
            void ResetDay()
            {
                m_requiredDocs.Clear();
                m_requiredCount = 0;
                this->AddRequiredDocument({ documents::DocType::RuleBook });
            }
            void ResetEntrant()
            {
                RequiredDocStatus* rulebook = nullptr;
                RequiredDocStatus copy;
                if (m_requiredDocs.Get({ documents::DocType::RuleBook }, rulebook))
                {
                    copy = *rulebook;
                    copy.hasWarnedNotAnalyzed = false;
                    copy.hasWarnedNotSpotted = false;
                }
                m_requiredDocs.Clear();
                if (rulebook != nullptr)
                {
                    m_requiredDocs.Set({ documents::DocType::RuleBook }, copy);
                }
            }

            // If there are required documents that have not been fully analyzed yet
            bool ExpectsMoreDocuments() const { return m_requiredCount != 0; }

            void SetExists(documents::AppearanceType appearanceType);
            //void SetSpotted(documents::AppearanceType appearanceType);
            void SetAnalyzed(documents::AppearanceType appearanceType);
        private:
            documents::DocLookup GetDocumentLookupType(documents::AppearanceType appearanceType) const noexcept;
            documents::DocLookup TryGetPassportLookupType()  const noexcept;

            void RemoveRequiredDocument(documents::DocLookup docLookup);

        private:
            core::FixedHashTable<documents::DocLookup, RequiredDocStatus, DocTypeSize> m_requiredDocs;
            size_t m_requiredCount = 0;
        };

    }
}
