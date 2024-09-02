#include "pch.h"
#include "base/game_state/components/game_state.h"

#include <iostream>

#include "base/utils/log.h"

namespace paplease {
	namespace analysis {
		namespace components {
			
			using namespace mediators;

#pragma region State Functions
bool GameState::IsNewDate(documents::data::Date date)
{
	if (date != m_currentDate)
	{
		LOG("New date!");
		m_currentDate = date;
		return true;
	}
	return false;
}

bool GameState::IsNewApplicant(int applicantNumber)
{
	if (applicantNumber != m_applicantNumber)
	{
		LOG("New applicant!");
		m_applicantNumber = applicantNumber;
		return true;
	}
	return false;
}

bool GameState::HasCurrentRuleBook()
{
	return m_hasCurrentRules;
}

bool GameState::HasCurrentCriminals()
{
	return m_hasCurrentCriminals;
}

#pragma endregion


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

void GameState::ReceiveRuleBookDocument(const documents::v2::Doc& document)
{
	
	auto ruleBook = CreateRuleBook(document);
	if (!ruleBook)
	{
		LOG_ERR("Rule book could not be loaded!");
		return;
	}
	LOG("Rule book has been loaded!");
	m_ruleBook = ruleBook.value();
	m_hasCurrentRules = true;
}

void GameState::ReceiveBulletinDocument(const documents::v2::Doc& document)
{
	LOG("Criminal data has been loaded!");
	m_criminalData = CreateCriminalData(document);
	m_hasCurrentCriminals = true;
}

void GameState::ReceiveTranscriptDocument(const documents::v2::Doc& document)
{
	LOG("Transcript has been loaded!");
	m_transcript = CreateTranscript(document);
	m_hasCurrentCriminals = true;
}


#pragma endregion

		}  // namespace components
	}  // namespace analysis
}  // namespace paplease
