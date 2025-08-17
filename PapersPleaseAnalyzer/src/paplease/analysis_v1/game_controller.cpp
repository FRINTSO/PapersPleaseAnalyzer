#include "pch.h"
#include "paplease/analysis_v1/game_controller.h"

namespace paplease {
	namespace analysis_v1 {

		using namespace documents;

#pragma region Constructors

		GameAnalysisController::GameAnalysisController(bool showGameView)
			: m_analysisContext{},
			m_showGameView{ showGameView }
		{}

#pragma endregion

		void GameAnalysisController::Update(const GameView& gameView)
		{
			auto scanContext = scannable::Scan(gameView);
			m_analysisContext.Update(scanContext);

			if (this->m_showGameView)
			{
				cv::imshow("booth", gameView.booth());
				cv::imshow("inspection", gameView.inspection());
				cv::waitKey();
			}
		}

	}  // namespace analysis_v1
}  // namespace paplease
