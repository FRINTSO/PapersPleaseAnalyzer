#include "pch.h"

#include <opencv2/core/utils/logger.hpp>

#include "base/game_state/game_controller.h"

#include "base/utils/log.h"


void main()
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	paplease::utils::Log::Init();

	paplease::analysis::GameAnalysisController analyzer{};
	paplease::GameView view;
	while (paplease::GetNextGameSimView("1", view))
	{
		analyzer.Update(view);
	}

	// Don't scan transcript until finished?
	cv::waitKey();
	std::cin.get();
}
