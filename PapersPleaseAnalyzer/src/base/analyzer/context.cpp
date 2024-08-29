#include "pch.h"
#include "base/analyzer/context.h"

void AnalysisContext::Update(const GameView& gameView)
{
	// Update game state
	m_gameState.Update(gameView);

	// update doc tracker
	m_docTracker.Update(gameView);
}