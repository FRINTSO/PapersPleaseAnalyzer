#include "pch.h"

#include <opencv2/core/utils/logger.hpp>

//#include "paplease/analysis/game_analyzer.h"
#include "paplease/analysis/analysis_statemachine.h"
#include "paplease/analysis_v1/game_controller.h"
#include "paplease/screencap/screencap.h"
#include "paplease/core/resource_manager.h"
#include "test/test_document_scanning.h"

#include "test/test_transcript.h"

static void RunWithSimulatedGame()
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

static void RunWithLiveGame()
{
	//paplease::analysis::GameAnalysisController analyzer{ false };
	// paplease::analysis::scannable::DocTracker tracker{};
	//paplease::core::ResourceManager resources;
	//paplease::analysis::GameAnalyzer analyzer{resources};
	//paplease::analysis::GameAnalyzer analyzer{};
	paplease::analysis::AnalysisStateMachine sm{};
	while (true)
	{
		auto result = paplease::screencap::CaptureGameWindow();
		auto resultOpt = paplease::screencap::ExtractGameWindow(result);
		if (!resultOpt.has_value())
		{
			std::cout << "couldn't extract view\n";
			continue;
		}
		result = *resultOpt;
		if (result.empty())
		{
			std::cout << "Empty\n";
			continue;
		}

		paplease::screencap::CalculatePixelGroupSize(result);

		cv::imshow("Win", result);
		cv::waitKey();

		paplease::GameView view(std::move(result));
		sm.Run(view);

		cv::waitKey(30);
	}
}

void main()
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	paplease::utils::Log::Init();

	//RunWithSimulatedGame();
	RunWithLiveGame();

	//test::test_transcript();
	std::cin.get();
}
