#include "pch.h"

#include <opencv2/core/utils/logger.hpp>

#include "base/analysis/game_controller.h"

#include "base/utils/log.h"

#include "test/documents/test_document_boxing.h"

void test()
{
	using namespace paplease::documents;

	auto path = "C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_sim\\3\\game_14.png";
	paplease::GameView game(path);

	auto doc = FindDocument(game, DocType::CertificateOfVaccination);
	auto data = doc->GetDocumentData();
	data.PrintAllFields();
}

void Run()
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

void main()
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	paplease::utils::Log::Init();

	
	// test();
	Run();

	std::cin.get();
}
