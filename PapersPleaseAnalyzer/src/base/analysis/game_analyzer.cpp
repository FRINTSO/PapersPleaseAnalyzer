#include "pch.h"
#include "base/analysis/game_analyzer.h"



namespace paplease {
	namespace analysis {

		void GameAnalyzer::Scan(const GameView& gameView)
		{
			m_currentScanContext = scannable::Scan(gameView);
		}

	}  // namespace analysis
}  // namespace paplease
