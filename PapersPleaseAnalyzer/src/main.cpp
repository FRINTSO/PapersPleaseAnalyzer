#include "pch.h"

#include <opencv2/core/utils/logger.hpp>

#include "paplease/analysis/game_analyzer.h"
#include "paplease/analysis_v1/game_controller.h"
#include "paplease/screencap/screencap.h"
#include "paplease/core/resource_manager.h"
#include "test/core/pool_test.h"


void RunWithSimulatedGame()
{
	paplease::analysis_v1::GameAnalysisController analyzer{ false };
	paplease::GameView view;
	while (paplease::GetNextGameSimView("1", view))
	{
		analyzer.Update(view);
	}

	// Don't scan transcript until finished?
	cv::waitKey();
}

void RunWithLiveGame()
{
	//paplease::analysis::GameAnalysisController analyzer{ false };
	// paplease::analysis::scannable::DocTracker tracker{};
	//paplease::core::ResourceManager resources;
	//paplease::analysis::GameAnalyzer analyzer{resources};
	paplease::analysis::GameAnalyzer analyzer{};
	while (true)
	{
		auto result = paplease::screencap::CaptureGameWindow();

		if (result.empty())
		{
			std::cout << "Empty\n";
			continue;
		}

		paplease::GameView view(std::move(result));
		analyzer.Scan(view);

		//tracker.Update(view);
		//analyzer.Update(view);
		//cv::imshow("output", result);
		cv::waitKey(30);
	}
}

void main()
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	paplease::utils::Log::Init();

	// RunWithSimulatedGame();
	RunWithLiveGame();

	//test::test_fixed_hash_table();

	std::cin.get();
}
