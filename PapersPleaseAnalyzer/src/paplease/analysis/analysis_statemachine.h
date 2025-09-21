#pragma once
#include "paplease/analysis/contexts/entrant_context.h"
#include "paplease/analysis/contexts/frame_context.h"
#include "paplease/analysis/contexts/game_context.h"
#include "paplease/analysis/engines/doc_engine.h"
#include "paplease/analysis/engines/rule_engine.h"
#include "paplease/analysis/engines/transcript_engine.h"
#include "paplease/analysis/required_docs_tracker.h"
#include "paplease/game_view.h"

namespace paplease {
    namespace analysis {

        /*
        Steps to analysis (Require document model):  - Require == Needs to analyze
        1. Require rulebook.
        
        */


        enum class FrameAnalysisResult
        {
            ContinueAnalysis,
            OnNewEntrant,
            OnNewDate,
            SkipAnalysis
        };

        class AnalysisStateMachine
        {
        public:
            AnalysisStateMachine()
                : m_game(), m_entrant(),
                m_docEngine(m_game, m_entrant, m_requiredDocsTracker),
                m_ruleEngine(m_entrant, m_requiredDocsTracker),
                m_transcriptEngine(),
                m_requiredDocsTracker()
            {}
            void Run(const GameView& gameView);

        private:
            void RunInternal(contexts::FrameContext& frame);
            FrameAnalysisResult RunAnalyzeFrame(contexts::FrameContext& frame);
            void RunDocumentAnalysis(contexts::FrameContext& frame);
            void RunDocumentAnalysisByRequiredDocumentsTracker(contexts::FrameContext& frame);
            void HandleDocumentAnalysisResult(engines::DocAnalysisResult result);
            void RunRuleEngine();

            void HandleNewRuleBook(data::RuleBook&& rulebook);
            void HandleNewCriminalData(data::CriminalData&& criminalData);
            void HandleNewTranscript(data::Transcript&& transcript);

            void OnNewDate(documents::data::Date date);
            void OnNewEntrant(int entrantNumber);

            void PromptUserAnalysisFinishState();

            void AttemptEntrantClassificationByProvidedDocument(documents::AppearanceType appearanceType);

        private:
            contexts::GameContext    m_game;
            contexts::EntrantContext m_entrant;

            engines::DocEngine  m_docEngine;
            engines::RuleEngine m_ruleEngine;
            engines::TranscriptEngine m_transcriptEngine;

            RequiredDocsTracker m_requiredDocsTracker;
        };

    }
}