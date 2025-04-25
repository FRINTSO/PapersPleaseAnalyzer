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



void Test()
{
	using namespace paplease::documents;
	
	auto path = "C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_sim\\3\\game_85.png";
	//auto path = "C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_sim\\3\\game_32.png";
	//auto path = "C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_sim\\3\\game_6.png";
	paplease::GameView game(path);

	auto result = paplease::scannable::ScanBooth(game);
	
	paplease::HSVConfig config{ 0, 179, 1, 70, 0, 65 };
	// test::documents::test_hsv(result->applicantHeadshot.m_mat, &config);
	cv::waitKey();
}

void testit(const std::string& path)
{
	std::cout << path << "\n";
	using namespace paplease::documents;
	using namespace paplease::documents::data;
	paplease::GameView game(path);
	auto boothData = paplease::scannable::ScanBooth(game);
	Photo boothPhoto = boothData.PhotoToBinaryHeadshotPhoto();
	// FIXUP BOOTH PHOTO
	auto sil = boothData.ExtractSilhouette();
	auto head = boothData.ExtractHead();
	auto face = boothData.ExtractFace();

	cv::Mat resultImg;
	//cv::bitwise_and(face, face, resultImg, head);
	//boothPhoto.m_mat =  sil | resultImg;

	Photo passportHeadshot = paplease::scannable::ScanForDocument(game, paplease::ViewArea::InspectionView, DocType::Passport)->ToDocument(game).GetDocumentData().GetFieldData<FieldCategory::Photo>()->get();
	// Photo idCardHeadshot = paplease::documents::FindDocument(game, DocType::IdentityCard)->GetDocumentData().GetFieldData<FieldCategory::Photo>()->get();

	bool result = paplease::analysis::IsSamePerson(passportHeadshot, boothPhoto);
	//bool result = paplease::analysis::IsSamePerson(passportHeadshot, idCardHeadshot);
	// result = paplease::analysis::IsSamePerson(boothPhoto, idCardHeadshot);
	std::cout << "Is same person: " << std::boolalpha << result << "\n";
	//cv::imshow("Booth", boothPhoto.m_mat);
	//cv::imshow("Passport", passportHeadshot.m_mat);
	cv::waitKey();
	cv::destroyAllWindows();
}

void test_is_same_person()
{
	std::cout << "valid photos:\n";
	//testit("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_4.png");
	testit("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_19.png");
	testit("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_46.png");
	testit("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_sim\\3\\game_97.png");
	//testit("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_sim\\3\\game_111.png");
	testit("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_sim\\3\\game_121.png");
	std::cout << "\n\n\ninvalid photos:\n";
	//testit("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_38.png");
	testit("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_sim\\3\\game_64.png");
}

void test_booth_face_extraction()
{
	namespace fs = std::filesystem;

	const fs::path folderPath = "C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\";
	paplease::HSVConfig config{};
	paplease::HSLConfig config2{};
	try
	{
		if (fs::exists(folderPath) && fs::is_directory(folderPath))
		{
			for (const auto& entry : fs::directory_iterator(folderPath))
			{
				if (fs::is_regular_file(entry.status()))
				{
					auto gameView = paplease::GameView(entry.path().string());
					auto result = paplease::scannable::ScanBooth(gameView);
					auto face = result.ExtractFace();
					//cv::imshow("Face", face);
					//cv::waitKey();
					//tests::documents::find_hsv(result->applicantHeadshot.m_mat, config);
					std::cout << entry.path().filename() << "\n";
					//tests::documents::find_hsl(result->applicantHeadshot.m_mat, config2);
				}
			}
		}
		else
		{
			std::cerr << "Invalid folder path.\n";
		}
	}
	catch (const fs::filesystem_error& e)
	{
		std::cerr << "Filesystem error: " << e.what() << '\n';
	}

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

void test_screencap()
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
	
	//test::test_faces();
	//test_is_same_person();
	//test();
	//Run();
	//test_booth_face_extraction();

	/*auto img = paplease::ScaleImage(cv::imread("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_52.png", cv::IMREAD_UNCHANGED), 0.4f);
	paplease::HSVConfig config{ 0 , 179 , 0 , 255 , 79 , 255 };
	test::documents::find_hsv(img, config);*/

	//test::test_rulebook();
	//test::test_transcript();
	test_screencap();
	//paplease::documents::test::test_document_character_boxing("29", paplease::documents::DocType::AccessPermit);

	std::cin.get();
}
