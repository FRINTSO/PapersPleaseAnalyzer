#include "pch.h"
#include "paplease/analysis/analysis_statemachine.h"

namespace paplease {
    namespace analysis {

        void AnalysisStateMachine::Run(const GameView& gameView)
        {
            scannable::ScanContext scanContext = scannable::Scan(gameView);
            auto frame = contexts::FrameContext{ gameView, scanContext };
            this->RunInternal(frame);
        }

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
                    break;
                }
                case FrameAnalysisResult::OnNewEntrant:
                {
                    // Toggle new entrant state
                    std::system("cls");
                    LOG_RAW("-----------------[ New Applicant ]-----------------");
                    break;
                }
                case FrameAnalysisResult::SkipAnalysis:
                {
                    return; // GameView is not in an analyzable state
                }
            }

            // Other states here
            // 
            // If rule book engine, run it
            if (m_ruleEngine.IsInitialized())
            {
                this->RunRuleEngine();
            }

            // Default case (always run)
            this->RunDocumentAnalysis(frame);
        }

        FrameAnalysisResult AnalysisStateMachine::RunAnalyzeFrame(contexts::FrameContext& frame)
        {
            // Update entrant context
            if (frame.weight.has_value() && !m_entrant.GetWeight().has_value())
            {
                m_entrant.SetWeight(*frame.weight);
            }

            if (frame.approximateHeight.has_value() && !m_entrant.GetApproximateHeight().has_value())
            {
                m_entrant.SetHeight(*frame.approximateHeight);
            }

            // Update game context
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

            return FrameAnalysisResult::ContinueAnalysis;
        }

        void AnalysisStateMachine::OnNewDate(documents::data::Date date)
        {
            m_game.Reset();
            m_game.SetCurrentDate(date);
            m_entrant.Reset();
        }

        void AnalysisStateMachine::OnNewEntrant(int entrantNumber)
        {
            m_entrant.Reset();
            m_game.SetEntrantNumber(entrantNumber);
        }

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

        void AnalysisStateMachine::HandleDocumentAnalysisResult(DocAnalysisResult result)
        {
            switch (result)
            {
                case DocAnalysisResult::CanBuildRuleBook:
                {
                    auto rulebookOpt = m_docEngine.TryBuildRuleBook();
                    if (!rulebookOpt) return;
                    this->HandleNewRuleBook(std::move(rulebookOpt.value()));
                    break;
                }
                case DocAnalysisResult::CanBuildTranscript:
                {
                    break;  // MISSING
                }
                case DocAnalysisResult::CanBuildCriminalData:
                {
                    break;  // MISSING
                }
            }
        }

        void AnalysisStateMachine::HandleNewRuleBook(data::RuleBook&& rulebook)
        {
            m_game.SetRuleBook(std::move(rulebook));
            const auto& rulebook_ = m_game.GetRuleBook();
            m_ruleEngine.Initialize(rulebook_);
            LOG("Rulebook initialized!");
        }

        void AnalysisStateMachine::HandleNewCriminalData(data::CriminalData&& criminalData)
        {
            LOG_WARN("NO IMPL: HandleNewCriminalData");
        }

        void AnalysisStateMachine::HandleNewTranscript(data::Transcript&& transcript)
        {
            LOG_WARN("NO IMPL: HandleNewTranscript");
        }


        void AnalysisStateMachine::RunRuleEngine()
        {
            m_ruleEngine.UpdateApplicableRules();
        }

    }
}
