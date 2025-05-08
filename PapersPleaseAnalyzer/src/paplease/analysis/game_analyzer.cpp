#include "pch.h"
#include "paplease/analysis/game_analyzer.h"
//
//#include "paplease/analysis/contexts/frame_context.h"
//
//namespace paplease {
//	namespace analysis {
//
//		void GameAnalyzer::Scan(const GameView& gameView)
//		{
//			scannable::ScanContext scanContext = scannable::Scan(gameView);
//			auto frame = contexts::FrameContext{ gameView, scanContext };
//			
//			// Check for new entrant
//			if (frame.entrantNumber && frame.entrantNumber != m_gameContext.GetEntrantNumber())
//			{
//				OnNewEntrant(frame.entrantNumber.value());
//			}
//
//			// Check for new date
//			if (frame.date && frame.date != m_gameContext.GetCurrentDate())
//			{
//				OnNewDate(frame.date.value());
//			}
//
//			m_documentPipeline.Run();
//		}
//
//		void GameAnalyzer::OnNewEntrant(int entrantNumber)
//		{
//			m_entrantContext.Reset();
//			m_gameContext.SetEntrantNumber(entrantNumber);
//
//			std::system("cls");
//			LOG_RAW("-----------------[ New Applicant ]-----------------");
//		}
//
//		void GameAnalyzer::OnNewDate(documents::data::Date date)
//		{
//			m_gameContext.Reset();
//			m_gameContext.SetCurrentDate(date);
//			m_entrantContext.Reset();
//
//			std::system("cls");
//			LOG_RAW("====================[ New Date ]====================");
//		}
//
//
//		//void GameAnalyzer::UpdateAnalysisContext()
//		//{
//		//	if (m_currentScanContext.boothData.date)
//		//	{
//		//		if (this->IsNewDate(m_currentScanContext.boothData.date.value()))
//		//		{
//		//			LOG_RAW("====================[ New Date ]====================");
//		//			this->OnNewDate();
//		//		}
//		//	}
//
//		//	if (m_currentScanContext.boothData.applicantNumber)
//		//	{
//		//		if (this->IsNewApplicant(m_currentScanContext.boothData.applicantNumber.value()))
//		//		{
//		//			std::system("cls");
//		//			LOG_RAW("-----------------[ New Applicant ]-----------------");
//		//			this->OnNewApplicant();
//		//		}
//		//	}
//		//	else
//		//	{
//		//		// Log an error if applicantNumber is missing
//		//		LOG_ERR("Missing boothData.applicantNumber! Cannot update the applicant.");
//		//	}
//
//		//	// Store booth data
//		//	m_analysisContext.currentDate = m_currentScanContext.boothData.date;
//		//	m_analysisContext.currentWeight = m_currentScanContext.boothData.weight;
//		//	m_analysisContext.approximateHeight = m_currentScanContext.boothData.approximateHeight;
//		//	m_analysisContext.applicantNumber = m_currentScanContext.boothData.applicantNumber.value_or(0);
//		//}
//		//	
//		//bool GameAnalyzer::IsNewDate(const documents::data::Date& date) const noexcept
//		//{
//		//	return date != m_analysisContext.currentDate;
//		//}
//
//		//bool GameAnalyzer::IsNewApplicant(int applicantNumber) const noexcept
//		//{
//		//	return applicantNumber != m_analysisContext.applicantNumber && applicantNumber != 0;
//		//}
//
//		//void GameAnalyzer::OnNewDate()
//		//{
//		//	m_analysisContext.OnNewDate();
//		//	m_documentPipeline.Clear();
//		//}
//
//		//void GameAnalyzer::OnNewApplicant()
//		//{
//		//	m_analysisContext.OnNewApplicant();
//		//	m_documentPipeline.Clear();
//		//}
//
//	}  // namespace analysis
//}  // namespace paplease
