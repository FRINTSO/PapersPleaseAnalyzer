#pragma once
#include "base/game_state/doc_tracker.h"
#include "base/game_state/game_state.h"
#include "base/game_view.h"

// Context of current analysis
// Stores context of current analysis
// Stores and keeps track of documents relating to current applicant
// Also stores the game state

class AnalysisContext
{
public:
	void RegisterDoc();
	void Update(const GameView& gameView); // represents one round in the analysis chain


private:
	DocTracker m_docTracker;
	GameState m_gameState;
};