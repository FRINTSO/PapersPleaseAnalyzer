#include "pch.h"

#include <opencv2/core/utils/logger.hpp>
#include <magic_enum/magic_enum.hpp>

#include "paplease/analysis/game_controller.h"
#include "paplease/analysis/game_analyzer.h"
#include "paplease/analysis/photo_analysis/photo_analyzer.h"
#include "paplease/screencap/screencap.h"
#include "paplease/utils/log.h"
#include "test/documents/test_document_boxing.h"
#include "test/documents/test_hsv.h"
#include "test/test_faces.h"
#include "test/test_rulebook.h"
#include "test/test_transcript.h"
#include "paplease/analysis/doc_tracker.h"
#include "paplease/core/fixed.h"
#include "test/documents/test_document_boxing.h"
#include <test/test_fixed.h>

void RunWithSimulatedGame()
{
	paplease::analysis::GameAnalysisController analyzer{ false };
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
