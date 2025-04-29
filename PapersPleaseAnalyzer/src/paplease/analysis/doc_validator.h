#pragma once
#include "paplease/analysis/analysis_context_v2.h"
#include "paplease/analysis/doc_store.h"
#include "paplease/documents/doc_view.h"
#include "paplease/game_view.h"

namespace paplease {
    namespace analysis {

        // Class for storing documents
        class DocValidator
        {
        public:
            constexpr DocValidator(DocStore& store, AnalysisContextV2& analysisContext)
                : m_store(store), m_context(analysisContext) {}

            bool ValidateDocument(documents::DocType documentType, const GameView& gameView);

        private:
            DocStore& m_store;
            AnalysisContextV2& m_context;
        };

    }  // namespace analysis
}  // namespace paplease
