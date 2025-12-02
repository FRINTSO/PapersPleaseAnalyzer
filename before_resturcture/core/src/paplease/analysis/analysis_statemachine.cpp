#include "pch.h"
#include "paplease/analysis/analysis_statemachine.h"

namespace paplease {
    namespace analysis {

        // ==== Entry Point ====
        void AnalysisStateMachine::Run(const GameView& gameView)
        {
            scannable::ScanContext scanContext = scannable::Scan(gameView);
            auto frame = contexts::FrameContext{ gameView, scanContext };
            this->RunInternal(frame);
        }

        // ==== Main Flow ====
        void AnalysisStateMachine::RunInternal(contexts::FrameContext& frame)
        {
            auto analysisResult = this->RunAnalyzeFrame(frame); // First step, to gather what needs to be done
            switch (analysisResult)
            {
                case FrameAnalysisResult::OnNewDate:
                {
                    // Toggle new date state
        			std::system("cls");
        			LOG_RAW("====================[ New Date ]====================");
                    return;
                }
                case FrameAnalysisResult::OnNewEntrant:
                {
                    // Toggle new entrant state
                    std::system("cls");
                    LOG_RAW("-----------------[ New Applicant ]-----------------");
                    return;
                }
                case FrameAnalysisResult::SkipAnalysis:
                {
                    return; // GameView is not in an analyzable state / or analysis is completed of current entrant
                }
            }

            // Try to locate and analyze documents in required documents list. Default value (at first) in this list, should be rulebook.
            // If required-document-to-analyze-list is empty, then I think we are done with the analysis
            // Required docs has two steps:
            // 1. First we need to see its DocView to verify that it is present
            // 2. Then we need to prompt the user to drag it into inspection view so we can analyze it.
            // If it is not visible, then we need to prompt the inspector, to demand the entrant to supply it

            // A required document can have this info
            // 1. Does it exist?  (For example, rulebook should have this implicitly set to true, since it is always available)
            // 2. Has it been seen?
            // 3. Has it been analyzed?

            // If rule book engine, run it
            if (m_ruleEngine.IsInitialized())
            {
                this->RunRuleEngine();
            }

            if (!m_requiredDocsTracker.ExpectsMoreDocuments())
            {
                this->PromptUserAnalysisFinishState();
                return;
            }

            // Continue analysis
            this->RunDocumentAnalysisByRequiredDocumentsTracker(frame);

            m_requiredDocsTracker.WarnMissingRequiredDocuments();
        }
        FrameAnalysisResult AnalysisStateMachine::RunAnalyzeFrame(contexts::FrameContext& frame)
        {
            // Update entrant context - only set once
            if (frame.weight.has_value() && !m_entrant.GetWeight().has_value())
            {
                m_entrant.SetWeight(*frame.weight);
            }

            if (frame.approximateHeight.has_value() && !m_entrant.GetApproximateHeight().has_value())
            {
                m_entrant.SetHeight(*frame.approximateHeight);
            }

            // Update game context - only set when new data
            if (frame.date && frame.date != m_game.GetCurrentDate())
            {
                this->OnNewDate(*frame.date);
                return FrameAnalysisResult::OnNewDate;
            }

            if (frame.entrantNumber && frame.entrantNumber != m_game.GetEntrantNumber())
            {
                this->OnNewEntrant(*frame.entrantNumber);
                return FrameAnalysisResult::OnNewEntrant;
            }

            // Don't continue
            if (m_entrant.AnalysisCompleted())
            {
                return FrameAnalysisResult::SkipAnalysis;
            }

            return FrameAnalysisResult::ContinueAnalysis;
        }

        // ==== State Change Handlers ====
        void AnalysisStateMachine::OnNewDate(documents::data::Date date)
        {
            m_game.Reset();
            m_game.SetCurrentDate(date);
            m_entrant.Reset();
            m_ruleEngine.ResetDay();
            m_requiredDocsTracker.ResetDay();
        }
        void AnalysisStateMachine::OnNewEntrant(int entrantNumber)
        {
            m_entrant.Reset();
            m_game.SetEntrantNumber(entrantNumber);
            m_ruleEngine.ResetEntrant();
            m_requiredDocsTracker.ResetEntrant();
        }

        // ==== Rule Engine ====
        void AnalysisStateMachine::RunRuleEngine()
        {
            m_ruleEngine.UpdateApplicableRules();
            
        }
        void AnalysisStateMachine::PromptUserAnalysisFinishState()
        {
            LOG("ANALYSIS OF CURRENT ENTRANT IS COMPLETE!");
            m_entrant.SetAnalysisCompleted();
        }

        // ==== Document Analysis ====
        void AnalysisStateMachine::RunDocumentAnalysis(contexts::FrameContext& frame)
        {  // document pipeline - process documents - add to profile - find discrepancies - etc.
            for (const auto& doc : frame.documents)
            {  // Here we can tag documents as found/scanned/needing scan depending on ViewArea and current scan status
                if (!m_docEngine.IsAnalyzable(doc, frame.gameView))
                {
                    continue;
                }

                auto result = m_docEngine.RunAnalysis(doc, frame.gameView);
                this->HandleDocumentAnalysisResult(result);
            }

        }
        void AnalysisStateMachine::RunDocumentAnalysisByRequiredDocumentsTracker(contexts::FrameContext& frame)
        {
            for (const auto& doc : frame.documents)
            {
                this->AttemptEntrantClassificationByProvidedDocument(doc.appearanceType);

                if (!m_requiredDocsTracker.Requires(doc.appearanceType))
                {
                    continue;  // only analyze required documents
                }

                m_requiredDocsTracker.SetExists(doc.appearanceType);

                if (!m_docEngine.IsAnalyzable(doc, frame.gameView))
                {
                    continue;
                }

                auto result = m_docEngine.RunAnalysis(doc, frame.gameView);
                if (result == engines::DocAnalysisResult::SuccessfulAnalysis)
                {
                    m_requiredDocsTracker.SetAnalyzed(doc.appearanceType);
                }
                else
                {
                    this->HandleDocumentAnalysisResult(result);
                }
            }
        }
        void AnalysisStateMachine::HandleDocumentAnalysisResult(engines::DocAnalysisResult result)
        {
            switch (result)
            {
                case engines::DocAnalysisResult::CanBuildRuleBook:
                {
                    auto rulebookOpt = m_docEngine.TryBuildRuleBook();
                    if (!rulebookOpt) return;
                    this->HandleNewRuleBook(std::move(rulebookOpt.value()));

                    // Clear rule book from required document here, but this should be moved later
                    m_requiredDocsTracker.SetAnalyzed(documents::AppearanceType::RuleBook);
                    break;
                }
                case engines::DocAnalysisResult::CanBuildTranscript:
                {
                    auto transcriptOpt = m_docEngine.TryBuildTranscript();
                    if (!transcriptOpt) return;
                    this->HandleNewTranscript(std::move(transcriptOpt.value()));

                    m_requiredDocsTracker.SetAnalyzed(documents::AppearanceType::Transcript);
                    break;
                }
                case engines::DocAnalysisResult::CanBuildCriminalData:
                {
                    break;  // MISSING
                }
            }
        }

        // ==== Data Handlers ====
        void AnalysisStateMachine::HandleNewRuleBook(data::RuleBook&& rulebook)
        {
            m_game.SetRuleBook(std::move(rulebook));
            const auto& rulebook_ = m_game.GetRuleBook();
            m_ruleEngine.Initialize(rulebook_);
            LOG("Rulebook initialized!");
        }
        void AnalysisStateMachine::HandleNewTranscript(data::Transcript&& transcript)
        {
            m_entrant.SetTranscript(std::move(transcript));
            const auto& transcript_ = m_entrant.GetTranscript();
            m_transcriptEngine.Initialize(transcript_);
            LOG_WARN("NO IMPL: HandleNewTranscript");
        }
        void AnalysisStateMachine::HandleNewCriminalData(data::CriminalData&& criminalData)
        {
            LOG_WARN("NO IMPL: HandleNewCriminalData");
        }

        // ==== Classification ====
        void AnalysisStateMachine::AttemptEntrantClassificationByProvidedDocument(documents::AppearanceType appearanceType)
        {
            if (!m_ruleEngine.IsInitialized())
            {
                return;
            }
            m_ruleEngine.ClassifyEntrantBasedOnDocument(appearanceType);
        }

    }  // namespace analysis
}  // namespace paplease
