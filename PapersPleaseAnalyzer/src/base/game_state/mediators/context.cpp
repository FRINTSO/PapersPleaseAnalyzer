#include "pch.h"
#include "base/game_state/mediators/context.h"

#include "base/utils/log.h"

#pragma region Constructors

AnalysisContext::AnalysisContext(bool showGameView)
	: m_gameState{ this }, m_boothState{ this }, m_inspectionState{ this }, m_profilerState{ this }, m_showGameView{ showGameView }
{}

AnalysisContext::~AnalysisContext()
{

}

#pragma endregion


#pragma region Mediator

void AnalysisContext::Notify(const Sender sender, const Event event)
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
			m_inspectionState.OnNewApplicant();
			break;
		}
		case Event::FindTranscript:
		{
			m_inspectionState.AddDocumentToLookout(Documents::V2::DocType::Transcript);
			break;
		}
		case Event::FindBulletin:
		{
			m_inspectionState.AddDocumentToLookout(Documents::V2::DocType::Bulletin);
			break;
		}
		case Event::FindRuleBook:
		{
			m_inspectionState.AddDocumentToLookout(Documents::V2::DocType::RuleBook);
			break;
		}
		default:
		{
			LOG_ERR("Unimplemented event!");
			break;
		}
	}
}

void AnalysisContext::NotifyDocumentFound(const Documents::V2::Doc& document)
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
				m_profilerState.ReceiveApplicantDocument(document);
			}
			else
			{
				LOG_ERR("Unimplemented document found in NotifyDocumentFound()!");
			}
			break;
		}
	}
	
}

#pragma endregion

void AnalysisContext::Update(const GameView& gameView)
{
	BeginLOG(AnalysisContext::Update());

	// Update booth
	// Update Booth State First, since we'll need it's data to direct game states update
	m_boothState.Update(gameView);

	// Update game state
	m_gameState.Update(gameView);

	// update doc tracker
	m_inspectionState.Update(gameView);

	m_profilerState.Update();

	if (this->m_showGameView)
	{
		cv::imshow("booth", gameView.booth);
		cv::imshow("inspection", gameView.inspection);
		cv::waitKey();
	}
	
	EndLOG(AnalysisContext::Update());
}