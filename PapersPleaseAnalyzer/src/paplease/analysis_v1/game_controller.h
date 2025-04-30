#pragma once
#include "paplease/analysis_v1/analysis_context.h"
#include "paplease/game_view.h"

// Context of current analysis
// Stores context of current analysis
// Stores and keeps track of documents relating to current applicant
// Also stores the game state

namespace paplease {
	namespace analysis_v1 {

		class GameAnalysisController
		{
		public:
			GameAnalysisController(bool showGameView = false);

			void Update(const GameView& gameView); // represents one round in the analysis chain
		private:
			AnalysisContext m_analysisContext;  // Context for all types of analysis

			// Temporary options - for development
			bool m_showGameView;
		};

	}  // namespace analysis_v1
}  // namespace paplease
