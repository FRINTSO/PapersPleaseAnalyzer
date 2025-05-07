#pragma once
#include "paplease/analysis/contexts/entrant_context.h"
#include "paplease/analysis/contexts/game_context.h"
#include "paplease/analysis/data/criminals.h"
#include "paplease/analysis/data/rules.h"
#include "paplease/analysis/data/transcript.h"
#include "paplease/analysis/required_docs_tracker.h"
#include "paplease/documents/doc_type.h"
#include "paplease/documents/doc_view.h"
#include "paplease/game_view.h"

#include <optional>

namespace paplease {
    namespace analysis {

        enum class DocAnalysisResult
        {
            NoAction,
            SuccessfulAnalysis,
            FailedAnalysis,
            CanBuildRuleBook,
            CanBuildTranscript,
            CanBuildCriminalData,
        };

        class DocEngine
        {
        public:
            DocEngine(contexts::GameContext& game, contexts::EntrantContext& entrant, RequiredDocsTracker& requiredDocsTracker)
                : m_game(game), m_entrant(entrant), m_requiredDocsTracker(requiredDocsTracker) {}

            bool IsAnalyzable(const documents::DocView& docView, const GameView& gameView);
            DocAnalysisResult RunAnalysis(const documents::DocView& docView, const GameView& gameView);

            std::optional<data::RuleBook>     TryBuildRuleBook();
            std::optional<data::Transcript>   TryBuildTranscript();
            std::optional<data::CriminalData> TryBuildCriminalData();

        private:
            bool RunAnalysisOnEntrantDocument(const documents::DocView& docView, const GameView& gameView);
            void RegisterDocData(documents::DocType documentType, documents::DocData&& data);

        private:
            contexts::GameContext&    m_game;
            contexts::EntrantContext& m_entrant;
            RequiredDocsTracker& m_requiredDocsTracker;
        };

    }
}