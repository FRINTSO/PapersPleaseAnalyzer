#include "pch.h"

#include <chrono>

#include <opencv2/core/utils/logger.hpp>

#include "base/game_state/mediators/game_controller.h"

#include "test/documents/test_document_boxing.h"
#include "test/documents/test_document_preprocessing.h"
//#include "test/documents/test_hsv.h"

#include "base/utils/log.h"


using namespace Documents::V2;

int main()
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	Utils::Log::Init();

	// GameView view = GetGameView("4");
	// auto doc = FindDocument(view.inspection, DocType::Passport);
	// auto data = doc.GetDocumentData();
	// data.Print();
	
	// Test::test_text_field_character_boxing("28", DocType::DiplomaticAuthorization, DataFieldCategory::CountryList);

	//Test::test_document_field_boxing("51", DocType::EntryPermit);
	//cv::waitKey();

	//Test::test_document_character_boxing("54", DocType::RuleBook);
	// Test::test_document_field_boxing("54", DocType::RuleBook);
	//Test::test_document_preprocessing("54", DocType::RuleBook);
	//Test::test_document_character_boxing("54", DocType::RuleBook);
	// Test::test_document_character_boxing("4", DocType::IdentityCard);

	//Documents::Test::test_hsv(GetGameView("57").inspection);
	// Test::test_document_preprocessing("35", DocType::Transcript);

	// Test::test_document_character_boxing("35", DocType::Transcript);

	GameAnalysisController context{false};
	GameView view;
	while (GetNextGameSimView("3", view))
	{
		context.Update(view);
	}

	// Don't scan transcript until finished?
	cv::waitKey();
}
