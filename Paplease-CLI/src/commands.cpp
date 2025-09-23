#include "commands.h"

#include <iostream>

#include <opencv2/opencv.hpp>

#include "paplease/analysis/analysis_statemachine.h"
#include "paplease/screencap/screencap.h"
// should be: paplease/core/screencap/screencap.h

namespace paplease {
	namespace cli {

		void help_command() {
			std::cout << "PapersPlease Analyzer CLI\n";
			std::cout << "Usage:\n";
			std::cout << "  paplease <command> [options]\n\n";
			std::cout << "Commands:\n";
			std::cout << "  help                   Show this help message\n";
			std::cout << "  live                   Runs paplease in live mode to track Papers, Please application in real time\n";
			std::cout << "\nFlags:\n";
			std::cout << "  -h, --help             Show help\n";
		}

		


		void live_command() {
			analysis::AnalysisStateMachine sm{};
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

				//paplease::screencap::CalculatePixelGroupSize(result);

				//cv::imshow("Win", result);
				//cv::waitKey();

				paplease::GameView view(std::move(result));
				sm.Run(view);

				cv::waitKey(30);
			}
		}

	}

}