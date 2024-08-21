#include "pch.h"

#include <opencv2/core/utils/logger.hpp>

#include "test/test_document_data_extraction.h"
#include "test/test_document_text_boxing.h"

#include "base/analyzer/analyzer.h"


int main()
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

	// GameView view = GetGameView("7");
	GameView view = GetGameView("20");
	Analyze(view);

	/*
	test_entry_permit_data_extraction("7");
	std::cout << "\n";
	test_passport_data_extraction("7");
	std::cout << "\n";
	test_identity_supplement_data_extraction("7");
	std::cout << "\n";
	test_certificate_of_vaccination_data_extraction("7");
	*/

	/*test_identity_card_text_boxing("47");
	test_identity_card_data_extraction("47");*/

	//test_certificate_of_vaccination_data_extraction("53");
	//test_certificate_of_vaccination_text_boxing("53");

	cv::waitKey();
	std::cin.get();
}
