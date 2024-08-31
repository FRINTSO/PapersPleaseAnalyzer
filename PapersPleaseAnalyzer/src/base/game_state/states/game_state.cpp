#include "pch.h"
#include "base/game_state/states/game_state.h"

#include <iostream>

#include "base/utils/log.h"

void GameState::Update(const GameView& gameView)
{
	BeginLOG(GameState::Update);




	// certain data only needs to update once per day
	// other data needs to update once per applicant

	EndLOG(GameState::Update);
}

#pragma region Mediator Functions

void GameState::OnNewGameDate()
{

	// require new date info
	// rules
	// criminals
	m_hasCurrentRules = false;
	m_hasCurrentCriminals = false;

	LOG("Load rule book and wait for scanning!");
	LOG("Load criminals and wait for scanning!");

	m_mediator->Notify(Sender::GameState, Event::FindRuleBook);
	m_mediator->Notify(Sender::GameState, Event::FindBulletin);
}

void GameState::OnNewApplicant()
{
	// require new applicant info
	// transcript
	// m_hasCurrentTranscript = false;
	LOG("Load transcript and wait for scanning!");

	m_mediator->Notify(Sender::GameState, Event::FindTranscript);
}

void GameState::ReceiveRuleBookDocument(const Documents::V2::Doc& document)
{
	LOG("Rule book has been loaded!");
	m_ruleBook = CreateRuleBook(document);
	m_hasCurrentRules = true;
}

void GameState::ReceiveBulletinDocument(const Documents::V2::Doc& document)
{
	LOG("Criminal data has been loaded!");
	m_criminalData = CreateCriminalData(document);
	m_hasCurrentCriminals = true;
}

void GameState::ReceiveTranscriptDocument(const Documents::V2::Doc& document)
{
	LOG("Transcript has been loaded!");
	m_transcript = CreateTranscript(document);
	m_hasCurrentCriminals = true;
}


#pragma endregion