#pragma once
#include "paplease/analysis/doc_tracker.h"
#include "paplease/game_view.h"
#include "paplease/scannable/scan_context.h"

namespace paplease {
	namespace analysis {

		class GameAnalyzer
		{
		public:
			void Scan(const GameView& gameView);

		private:
			// Per scan
			const GameView* m_currentGameView = nullptr;
			scannable::ScanContext m_currentScanContext;

			// States
			DocTrackerV2 m_documentTracker;
		};

	}  // namespace analysis
}  // namespace paplease
