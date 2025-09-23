#include <iostream>

#include <opencv2/core/utils/logger.hpp>

//#include "paplease/cli/commands.h" -- it should be this
#include "paplease/utils/log.h"

#include "commands.h"

using namespace paplease::cli;

int main(int argc, char* argv[]) {
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	paplease::utils::Log::Init();
	live_command();
	if (argc < 2) {
		help_command();
		return 1;
	}

	std::string first_arg = argv[1];

	// Check for help flags first
	if (first_arg == "--help" || first_arg == "-h" || first_arg == "help") {
		help_command();
		return 0;
	}

	std::string &command = first_arg;

	if (command == "live") {
		live_command();
	} else {
		std::cout << "Unknown command: " << command << "\n";
	}

	return 0;
}