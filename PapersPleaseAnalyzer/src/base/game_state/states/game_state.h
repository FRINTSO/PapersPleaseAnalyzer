#pragma once
#include "base/documents_v2/doc_class.h"
#include "base/game_state/criminals.h"
#include "base/game_state/rules.h"
#include "base/game_state/states/state.h"
#include "base/game_state/transcript.h"
#include "base/game_view.h"

// Tracks the games state
// Including booth data
// Includes game rules for chosen day
//

class BoothDoc;


class GameState final : public State
{
public:
	using State::State;

	void Update(const GameView& gameView);

public: // Mediator functions
	void OnNewGameDate();
	void OnNewApplicant();

	void ReceiveRuleBookDocument(const Documents::V2::Doc& document);
	void ReceiveBulletinDocument(const Documents::V2::Doc& document);
	void ReceiveTranscriptDocument(const Documents::V2::Doc& document);
private:
	// State
	bool m_hasCurrentRules;
	bool m_hasCurrentCriminals;

	RuleBook m_ruleBook;
	CriminalData m_criminalData;
	Transcript m_transcript;
};