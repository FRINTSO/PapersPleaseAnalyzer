#include "pch.h"
#include "paplease/analysis/game_analyzer.h"

namespace paplease {
	namespace analysis {

		void GameAnalyzer::Scan(const GameView& gameView)
		{
			m_currentGameView = &gameView;
			m_currentScanContext = scannable::Scan(gameView);
			UpdateAnalysisContext();
			m_documentPipeline.Process(m_currentScanContext);
		}

		void GameAnalyzer::UpdateAnalysisContext()
		{
			if (m_currentScanContext.boothData.date)
			{
				if (this->IsNewDate(m_currentScanContext.boothData.date.value()))
				{
					LOG_RAW("====================[ New Date ]====================");
					this->OnNewDate();
				}
			}

			if (m_currentScanContext.boothData.applicantNumber)
			{
				if (this->IsNewApplicant(m_currentScanContext.boothData.applicantNumber.value()))
				{
					std::system("cls");
					LOG_RAW("-----------------[ New Applicant ]-----------------");
					this->OnNewApplicant();
				}
			}
			else
			{
				// Log an error if applicantNumber is missing
				LOG_ERR("Missing boothData.applicantNumber! Cannot update the applicant.");
			}

			// Store booth data
			m_analysisContext.currentDate = m_currentScanContext.boothData.date;
			m_analysisContext.currentWeight = m_currentScanContext.boothData.weight;
			m_analysisContext.approximateHeight = m_currentScanContext.boothData.approximateHeight;
			m_analysisContext.applicantNumber = m_currentScanContext.boothData.applicantNumber.value_or(0);
		}
			
		bool GameAnalyzer::IsNewDate(const documents::data::Date& date) const noexcept
		{
			return date != m_analysisContext.currentDate;
		}

		bool GameAnalyzer::IsNewApplicant(int applicantNumber) const noexcept
		{
			return applicantNumber != m_analysisContext.applicantNumber;
		}

		void GameAnalyzer::OnNewDate()
		{
			m_analysisContext.OnNewDate();
			m_documentPipeline.Clear();
		}

		void GameAnalyzer::OnNewApplicant()
		{
			m_analysisContext.OnNewApplicant();
			m_documentPipeline.Clear();
		}

	}  // namespace analysis
}  // namespace paplease
