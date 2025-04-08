#include "pch.h"
#include "base/analysis/game_controller.h"

#include <utility>

#include "base/utils/log.h"

namespace paplease {
	namespace analysis {

		using namespace documents;
		using namespace mediators;

#pragma region Constructors

		GameAnalysisController::GameAnalysisController(bool showGameView)
			: m_analysisContext{},
			m_showGameView{ showGameView }
		{}

		GameAnalysisController::~GameAnalysisController()
		{

		}

#pragma endregion

		void GameAnalysisController::Update(const GameView& gameView)
		{
			BeginLOG("GameAnalysisController::Update()");

			// Scan Booth
			// Scan Inspection

			// Update Game State
			// Update profile
			auto scanContext = scannable::Scan(gameView);
			if (!scanContext)
			{
				LOG_ERR("UpdateGameState Failed!");
				EndLOG("GameAnalysisController::Update()");
				return;
			}

			m_analysisContext.Update(*scanContext);

			if (this->m_showGameView)
			{
				cv::imshow("booth", gameView.booth);
				cv::imshow("inspection", gameView.inspection);
				cv::waitKey();
			}
			EndLOG("GameAnalysisController::Update()");
		}

#if OLD
		void GameAnalysisController::UpdateGameState()
		{
			BeginLOG("GameAnalysisController::UpdateGameState()");

			if (m_documentState.HasRuleBook() && !m_gameState.HasCurrentRuleBook())
			{
				const auto& ruleBook = m_documentState.GetDocumentByType(DocType::RuleBook);
				m_gameState.ReceiveRuleBookDocument(ruleBook.value());
			}

			if (m_documentState.HasCriminals() && !m_gameState.HasCurrentCriminals())
			{
				const auto& bulletin = m_documentState.GetDocumentByType(DocType::Bulletin);
				m_gameState.ReceiveBulletinDocument(bulletin.value());
			}
			EndLOG("GameAnalysisController::UpdateGameState()");
		}
#endif

#if OLD
		bool GameAnalysisController::UpdateBooth(const GameView& gameView)
		{
			BeginLOG("GameAnalysisController::UpdateBooth()");

			auto boothData = m_booth.Scan(gameView);
			if (!boothData)
			{
				LOG_ERR("Booth couldn't scan game view!");
				EndLOG("GameAnalysisController::UpdateBooth()");
				return false;
			}

			// Check new date
			if (m_gameState.IsNewDate(boothData->date))
			{
				m_gameState.OnNewGameDate();
				m_documentState.ClearAll();
			}

			if (m_gameState.IsNewApplicant(boothData->applicantNumber))
			{
				m_gameState.OnNewApplicant();
				m_documentState.ClearApplicantSession();
				m_profiler.ClearAll();
			}

			EndLOG("GameAnalysisController::UpdateBooth()");
			return true;
		}

		void GameAnalysisController::UpdateInspection(const GameView& gameView)
		{
			BeginLOG("GameAnalysisController::UpdateInspection()");

			m_documentState.ClearNewlyRegistered();

			auto documents = m_inspection.Scan(gameView);

			for (auto& document : documents)
			{
				if (document.GetDocumentType() == documents::DocType::Invalid)
				{
					break;
				}
				m_documentState.RegisterIfNewDocument(std::move(document));
			}

			EndLOG("GameAnalysisController::UpdateInspection()");
		}
#endif

#if OLD
		void GameAnalysisController::DocumentAnalysis()
		{
			BeginLOG("GameAnalysisController::DocumentAnalysis()");

			for (const auto documentType : m_documentState.GetNewlyRegisteredTypes())
			{
				if (IsApplicantDocument(documentType))
				{
					auto document = m_documentState.GetDocumentByType(documentType);

					assert(document.has_value());

					auto analysis = m_analyzer.AnalyzeDocumentValidity(document.value());

					if (analysis.hasDiscrepancy)
					{
						LOG_ERR("Document of type '{}' has discrepancy", ToStringView(documentType));
					}

					m_profiler.AddDocumentToProfile(document.value());
				}
			}

			EndLOG("GameAnalysisController::DocumentAnalysis()");
		}
#endif

#pragma region Mediator

		void GameAnalysisController::Notify(const Sender sender, const Event event)
		{
			switch (event)
			{
				case Event::NewGameDate:
				{
					//m_gameState.OnNewGameDate();
					//m_documentState.ClearAll();
					// m_applicantState.ClearAll();
					break;
				}
				case Event::NewApplicant:
				{
					//m_gameState.OnNewApplicant();
					//m_documentState.ClearApplicantSession();
					//m_applicantState.ResetProfile();
					break;
				}
				case Event::FindTranscript:
				{
					//m_inspection.AddDocumentToLookout(documents::DocType::Transcript);
					break;
				}
				case Event::FindBulletin:
				{
					//m_inspection.AddDocumentToLookout(documents::DocType::Bulletin);
					//m_analyzer.OnBulletinFound();
					break;
				}
				case Event::FindRuleBook:
				{
					//m_inspection.AddDocumentToLookout(documents::DocType::RuleBook);
					break;
				}
				default:
				{
					LOG_ERR("Unimplemented event!");
					break;
				}
			}
		}

		void GameAnalysisController::NotifyDocumentFound(const Doc& document)
		{
			switch (document.GetDocumentType())
			{
				case DocType::Bulletin:
				{
					//m_gameState.ReceiveBulletinDocument(document);
					break;
				}
				case DocType::RuleBook:
				{
					//m_gameState.ReceiveRuleBookDocument(document);
					break;
				}
				case DocType::Transcript:
				{
					//m_gameState.ReceiveTranscriptDocument(document);
					break;
				}
				default:
				{
					if (IsApplicantDocument(document.GetDocumentType()))
					{
						// m_analyzer.QueueDocumentAnalysis(document) // .AnalyzeDocumentValidity(document); // don't know if i have the means for the analysis as of now
						// m_profilerState.ReceiveApplicantDocument(document);
						//m_applicantState.AddDocumentToProfile(document);
					}
					else
					{
						LOG_ERR("Unimplemented document '{}' found in NotifyDocumentFound()!", ToStringView(document.GetDocumentType()));
					}
					break;
				}
			}

		}

		const documents::Data& GameAnalysisController::RequestData(const documents::DataType dataType) const
		{
			switch (dataType)
			{
				case paplease::documents::DataType::Date:
				{
					//auto date = m_gameState.GetCurrentDate();
					//return Data{ date };
				}
				default:
					break;
			}
			// throw "err";
			LOG_ERR("WOW A DATE! not really");
			return {};
		}


#pragma endregion

	}  // namespace analysis
}  // namespace paplease
