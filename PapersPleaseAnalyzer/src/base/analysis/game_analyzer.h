#pragma once
#include "base/analysis/scannable/scan_context.h"
#include "base/game_view.h"

namespace paplease {
	namespace analysis {

		class GameAnalyzer
		{
		public:
			void Scan(const GameView& gameView);

		private:
			scannable::ScanContext m_currentScanContext;
		};

	}  // namespace analysis
}  // namespace paplease
