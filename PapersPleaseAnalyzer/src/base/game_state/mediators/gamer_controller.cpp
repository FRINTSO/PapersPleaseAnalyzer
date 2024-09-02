#include "pch.h"
#include "base/game_state/mediators/game_controller.h"

#include <utility>

#include "base/utils/log.h"

using namespace Documents::V2;

#pragma region Constructors

GameAnalysisController::GameAnalysisController(bool showGameView)
	: m_gameState{ this },
	m_documentState{},
	m_booth{},
	m_inspection{ this },
	m_profilerState{ this },
	m_analyzer{ this },
	m_showGameView{ showGameView }
{}

GameAnalysisController::~GameAnalysisController()
{

}

#pragma endregion

void GameAnalysisController::Update(const GameView& gameView)
{
	BeginLOG("GameAnalysisController::Update()");

	// Update Booth State First, since we'll need it's data to direct game states update
	this->UpdateBooth(gameView);

	// update inspection
	this->UpdateInspection(gameView);

	this->UpdateGameState();
	
	m_profilerState.Update();


	if (this->m_showGameView)
	{
		cv::imshow("booth", gameView.booth);
		cv::imshow("inspection", gameView.inspection);
		cv::waitKey();
	}

	EndLOG("GameAnalysisController::Update()");
}

void GameAnalysisController::UpdateBooth(const GameView& gameView)
{
	BeginLOG("GameAnalysisController::UpdateBooth()");

	auto boothData = m_booth.Scan(gameView.booth);
	if (!boothData)
	{
		LOG_ERR("Booth couldn't scan game view!");
		return;
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
	}

	EndLOG("GameAnalysisController::UpdateBooth()");
}

void GameAnalysisController::UpdateInspection(const GameView& gameView)
{
	BeginLOG("GameAnalysisController::UpdateInspection()");

	auto documents = m_inspection.Scan(gameView);
	
	for (auto& document : documents)
	{
		if (document.GetDocumentType() == Documents::V2::DocType::Invalid)
		{
			break;
		}
		m_documentState.RegisterIfNewDocument(std::move(document));
	}

	EndLOG("GameAnalysisController::UpdateInspection()");
}

void GameAnalysisController::UpdateGameState()
{
	BeginLOG("GameAnalysisController::UpdateGameState()");

	if (m_documentState.HasRuleBook() && !m_gameState.HasCurrentRuleBook())
	{
		const auto& ruleBook = m_documentState.GetDocumentByType(DocType::RuleBook);
		m_gameState.ReceiveRuleBookDocument(ruleBook.value());
		// LOG("Rule book has been found!");
	}

	if (m_documentState.HasCriminals() && !m_gameState.HasCurrentCriminals())
	{
		const auto& bulletin = m_documentState.GetDocumentByType(DocType::Bulletin);
		m_gameState.ReceiveBulletinDocument(bulletin.value());
		// LOG("Criminal bulletin board has been found!");
	}
	EndLOG("GameAnalysisController::UpdateGameState()");
}

#pragma region Mediator

void GameAnalysisController::Notify(const Sender sender, const Event event)
{
	switch (event)
	{
		case Event::NewGameDate:
		{
			m_gameState.OnNewGameDate();
			break;
		}
		case Event::NewApplicant:
		{
			m_gameState.OnNewApplicant();
			//m_inspection.OnNewApplicant();
			break;
		}
		case Event::FindTranscript:
		{
			//m_inspection.AddDocumentToLookout(Documents::V2::DocType::Transcript);
			break;
		}
		case Event::FindBulletin:
		{
			//m_inspection.AddDocumentToLookout(Documents::V2::DocType::Bulletin);
			//m_analyzer.OnBulletinFound();
			break;
		}
		case Event::FindRuleBook:
		{
			//m_inspection.AddDocumentToLookout(Documents::V2::DocType::RuleBook);
			break;
		}
		default:
		{
			LOG_ERR("Unimplemented event!");
			break;
		}
	}
}

void GameAnalysisController::NotifyDocumentFound(const Documents::V2::Doc& document)
{
	switch (document.GetDocumentType())
	{
		case Documents::V2::DocType::Bulletin:
		{
			m_gameState.ReceiveBulletinDocument(document);
			break;
		}
		case Documents::V2::DocType::RuleBook:
		{
			m_gameState.ReceiveRuleBookDocument(document);
			break;
		}
		case Documents::V2::DocType::Transcript:
		{
			m_gameState.ReceiveTranscriptDocument(document);
			break;
		}
		default:
		{
			if (Documents::V2::IsApplicantDocument(document.GetDocumentType()))
			{
				// m_analyzer.QueueDocumentAnalysis(document) // .AnalyzeDocumentValidity(document); // don't know if i have the means for the analysis as of now
				// m_profilerState.ReceiveApplicantDocument(document);
			}
			else
			{
				LOG_ERR("Unimplemented document found in NotifyDocumentFound()!");
			}
			break;
		}
	}
	
}

const Documents::Data::Date& GameAnalysisController::RequestCurrentDate()
{
	throw "err";
}

#pragma endregion

