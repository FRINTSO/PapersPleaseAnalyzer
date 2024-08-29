#pragma once
#include "base/document_data/date.h"
#include "base/document_data/field_data.h"
#include "base/document_data/photo.h"
#include "base/game_state/booth_state.h"
#include "base/game_state/criminals.h"
#include "base/game_state/rules.h"
#include "base/game_state/transcript.h"
#include "base/game_state/doc_tracker.h"
#include "base/game_view.h"

// Tracks the games state
// Including booth data
// Includes game rules for chosen day
//

class BoothDoc;


class GameState
{
public:
	void Update(const GameView& gameView);

	const BoothState& GetBoothState() const;
	Transcript GetTranscript() const;
	RuleBook GetRuleBook() const;
	CriminalData GetCriminals() const;
private:
	BoothState m_booth;
	DocTracker m_docTracker;
};