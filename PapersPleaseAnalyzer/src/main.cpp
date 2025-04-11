#include "pch.h"

#include <opencv2/core/utils/logger.hpp>

#include "base/analysis/game_controller.h"
#include "base/analysis/photo_analysis/applicant_analysis.h"
#include "test/documents/test_hsv.h"

#include "base/utils/log.h"

#include "test/documents/test_document_boxing.h"

void test()
{
	using namespace paplease::documents;

	auto path = "C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_sim\\3\\game_85.png";
	//auto path = "C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_sim\\3\\game_32.png";
	//auto path = "C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_sim\\3\\game_6.png";
	paplease::GameView game(path);

	auto result = paplease::analysis::scannable::ScanBooth(game);
	
	tests::documents::test_hsv(result->m_applicantHeadshot.m_mat, 0, 179, 1, 70, 0, 65);
	cv::waitKey();
}

void Run()
{
	paplease::analysis::GameAnalysisController analyzer{ false };
	paplease::GameView view;
	while (paplease::GetNextGameSimView("3", view))
	{
		analyzer.Update(view);
	}

	// Don't scan transcript until finished?
	cv::waitKey();
}

void main()
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	paplease::utils::Log::Init();
	
	//test();
	Run();

	std::cin.get();
}
