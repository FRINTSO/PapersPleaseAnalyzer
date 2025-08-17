#pragma once
#include "paplease/analysis/contexts/entrant_context.h"
#include "paplease/documents/doc_view.h"
#include "paplease/game_view.h"

namespace paplease {
    namespace analysis {

        // Class for storing documents
        class DocValidator
        {
        public:
            constexpr DocValidator(contexts::EntrantContext& entrant)
                : m_entrant(entrant) {}

            bool ValidateDocument(documents::DocType documentType, const GameView& gameView);
            bool ValidateDocument(documents::DocView& docView, const GameView& gameView);

        private:
            contexts::EntrantContext& m_entrant;
        };

    }  // namespace analysis
}  // namespace paplease
