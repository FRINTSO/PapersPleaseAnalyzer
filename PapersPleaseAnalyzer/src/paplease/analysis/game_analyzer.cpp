#include "pch.h"
#include "paplease/analysis/game_analyzer.h"

namespace paplease {
	namespace analysis {

		void GameAnalyzer::Scan(const GameView& gameView)
		{
			m_currentGameView = &gameView;
			m_currentScanContext = scannable::Scan(gameView);

			m_documentTracker.RefreshTracking(m_currentScanContext);
		}

	}  // namespace analysis
}  // namespace paplease
