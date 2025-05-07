#pragma once
#include "paplease/core/fixed.h"
#include "paplease/documents/doc_type.h"

namespace paplease {
    namespace analysis {

        struct DocLookup
        {
            documents::DocType documentType;
            documents::PassportType passportType = documents::PassportType::Invalid;

            constexpr DocLookup()
                : documentType(documents::DocType::Invalid), passportType(documents::PassportType::Invalid) {}
            constexpr DocLookup(documents::DocType documentType)
                : documentType(documentType) {}
            constexpr DocLookup(documents::DocType documentType, documents::PassportType passportType)
                : documentType(documentType), passportType(passportType) {}
            constexpr DocLookup(documents::AppearanceType appearanceType)
                : documentType(documents::ToDocType(appearanceType)), passportType(documents::ToPassportType(appearanceType))
            {}

            constexpr bool IsValid() const noexcept { return documentType != documents::DocType::Invalid; }
            constexpr bool IsPassport() const noexcept { return documentType == documents::DocType::Passport; }
            std::string ToString() const noexcept;
        };

        constexpr bool operator==(const DocLookup& left, const DocLookup& right)
        {
            return left.documentType == right.documentType && left.passportType == right.passportType;
        }

        struct RequiredDocStatus
        {
            RequiredDocStatus()
                : lookupType(documents::AppearanceType::Invalid),
                exists(false),
                analyzed(false)
            {}
            RequiredDocStatus(DocLookup lookupType)
                : lookupType(lookupType),
                exists(documents::IsEntrantDocument(lookupType.documentType) ? false : true),
                // spotted(false),
                analyzed(false)
            {}

            DocLookup lookupType;
            bool exists;  // if it exists, then we can prompt the user to analyze it. Can't do that if it doesn't exist. Rulebook, Transcript, Bulletin always exists
            bool analyzed;

            // logging fields
            bool hasWarnedNotSpotted = false;
            bool hasWarnedNotAnalyzed = false;
        };

        class RequiredDocsTracker
        {
            static constexpr size_t DocTypeSize = static_cast<size_t>(documents::DocType::Count);
            using DocLookupTypes = core::FixedHashSet<DocLookup, DocTypeSize>;
        public:
            RequiredDocsTracker() noexcept
                : m_requiredDocs()
            {
                this->AddRequiredDocument({ documents::DocType::RuleBook });
            }

            void AddRequiredDocument(DocLookup docLookup);
            bool Requires(documents::AppearanceType appearanceType) const noexcept;

            void WarnMissingRequiredDocuments();

            // Get documents that are required, but that haven't been analyzed
            core::FixedArray<RequiredDocStatus, DocTypeSize> GetNonAnalyzedRequiredDocuments() const;
            void ResetDay()
            {
                m_requiredDocs.Clear();
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
            bool ExpectsMoreDocuments() { return m_requiredDocs.Count() != 0; }

            void SetExists(documents::AppearanceType appearanceType);
            //void SetSpotted(documents::AppearanceType appearanceType);
            void SetAnalyzed(documents::AppearanceType appearanceType);
        private:
            DocLookup GetDocumentLookupType(documents::AppearanceType appearanceType) const noexcept;
            DocLookup TryGetPassportLookupType()  const noexcept;

            void RemoveRequiredDocument(DocLookup docLookup);

        private:
            //DocLookupTypes m_requiredLookupTypes;
            //core::FixedHashTable<DocLookup, RequiredDocStatus, DocTypeSize> m_documentStatuses;
            core::FixedHashTable<DocLookup, RequiredDocStatus, DocTypeSize> m_requiredDocs;
        };

    }
}
#include <functional>
namespace std {
    template <>
    struct hash<paplease::analysis::DocLookup>
    {
        size_t operator()(const paplease::analysis::DocLookup& key) const noexcept
        {
            using doc_utype = std::underlying_type_t<paplease::documents::DocType>;
            using pass_utype = std::underlying_type_t<paplease::documents::PassportType>;

            size_t h1 = std::hash<doc_utype>{}(static_cast<doc_utype>(key.documentType));
            size_t h2 = std::hash<pass_utype>{}(static_cast<pass_utype>(key.passportType));
            return h1 ^ (h2 << 1); // Standard combine pattern
        }
    };
}