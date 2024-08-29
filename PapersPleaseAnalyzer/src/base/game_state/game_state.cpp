#include "pch.h"
#include "base/game_state/game_state.h"

#include <iostream>

void GameState::Update(const GameView& gameView)
{
	std::cout << "Begin Game State Update\n";
	// Update Booth State First, since we'll need it's data to direct our update
	m_booth.Update(gameView);

	// certain data only needs to update once per day
	// other data needs to update once per applicant

	// Update documents
	m_docTracker.Update(gameView);

	// Do analysis
	

	std::cout << "End Game State Update\n\n";
}