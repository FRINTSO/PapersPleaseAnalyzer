#pragma once
#include "paplease/core/fixed.h"
#include "paplease/documents/doc_type.h"

namespace paplease {
    namespace analysis {

        struct DocLookup
        {
            documents::DocType documentType;
            documents::PassportType passportType = documents::PassportType::Invalid;

            DocLookup(documents::DocType documentType)
                : documentType(documentType) {}
            DocLookup(documents::DocType documentType, documents::PassportType passportType)
                : documentType(documentType), passportType(passportType) {}
            DocLookup(documents::AppearanceType appearanceType)
                : documentType(documents::ToDocType(appearanceType)), passportType(documents::ToPassportType(appearanceType))
            {}
        };

        class RequiredDocsTracker
        {
        public:
            using DocLookupTypes = core::FixedHashSet<DocLookup, static_cast<size_t>(documents::DocType::Count)>;

            void AddRequiredDocument(DocLookup docLookup);
            DocLookupTypes GetMissingDocuments();
            void Reset();
        private:
        };

    }
}