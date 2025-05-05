#pragma once

namespace paplease {
    namespace analysis {

        class DocRegistry
        {
        public:
            void AddSeenDocument(DocLookup lookup);
            void AddRequiredDocument();
            void GetDocumentsMissing();
        };

    }
}