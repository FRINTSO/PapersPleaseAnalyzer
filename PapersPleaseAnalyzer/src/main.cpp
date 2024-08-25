#include "pch.h"

#include <opencv2/core/utils/logger.hpp>

#include "test/documents/test_document_data_extraction.h"
#include "test/documents/test_document_boxing.h"
#include "test/documents/test_document_preprocessing.h"

#include "base/analyzer/analyzer.h"
#include "base/documents_v2/doc_class.h"
#include "base/documents_v2/doc_layout.h"
#include "base/ocr/font.h"
#include "base/image_process.h"

using namespace Documents::V2;

int main()
{
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);
	
	// GameView view = GetGameView("4");
	// auto doc = FindDocument(view.inspection, DocType::Passport);
	// auto data = doc.GetDocumentData();
	// data.Print();
	
	// Test::test_text_field_character_boxing("28", DocType::DiplomaticAuthorization, DataFieldCategory::CountryList);

	//Test::test_document_field_boxing("51", DocType::EntryPermit);
	//cv::waitKey();

	for (size_t i = 1; i < 54; i++)
	{
		std::cout << i << "\n";
		Test::test_document_field_boxing(std::to_string(i), DocType::WorkPass);
		
	}

	cv::waitKey();
}
