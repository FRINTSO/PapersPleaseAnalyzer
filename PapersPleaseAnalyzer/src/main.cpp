#include "pch.h"

#include <opencv2/core/utils/logger.hpp>

#include "test/test_document_data_extraction.h"
#include "test/test_document_text_boxing.h"
#include "test/test_document_preprocessing.h"

#include "base/image_process.h"
#include "base/analyzer/analyzer.h"
#include "base/documents_v2/doc_class.h"
#include "base/documents_v2/doc_layout.h"

inline void test(const Documents::V2::DocData& data, size_t i) {
	std::cout << data.GetData()[i].str << "\n";
}

inline bool test2(const Documents::V2::DocData& data, size_t i) {
	return (int)data.GetData()[i].category & 1;
}

int main()
{
	// std::cout << thing << "\n";
	cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);

	// auto test = DocAppearance{};
	// GameView view = GetGameView("7");
	// GameView view = GetGameView("20");
	// GameView view = GetGameView("7");
	// auto test = FindDocument(view.inspection, DocType::EntryTicket);
	//Analyze(view);

	// auto result = Documents::V2::DocLayout::Get(AppearanceType::Passport_Arstotzka);

	// std::cout << &result << "\n";

	// test_document_preprocessing("1", Documents::V2::DocType::Passport);
	// test_document_text_boxing("4", Documents::V2::DocType::Passport);
	cv::waitKey();
	//test_document_text_boxing("4", Documents::V2::DocType::IdentityCard);


	GameView view = GetGameView("43");
	auto docs = Documents::V2::FindAllDocuments(view.inspection);
	
	for (size_t i = 0; i < Documents::V2::DocData::ARRAY_LENGTH; i++) {
		for (const auto& doc : docs) {
			std::cout << Documents::V2::DataFieldCategoryAsString(static_cast<Documents::V2::DataFieldCategory>(i)) << "\n";
			auto data = doc.GetDocumentData();
			test(data, i);
			std::cout << "----------\n";
		}
		// }
	}

	/*
	auto passport = Documents::V2::FindDocument(view.inspection, Documents::V2::DocType::Passport);
	auto workpass = Documents::V2::FindDocument(view.inspection, Documents::V2::DocType::IdentityCard);

	auto passData = passport.GetDocumentData();
	auto data = workpass.GetDocumentData();

	for (size_t i = 0; i < Documents::V2::DocData::ARRAY_LENGTH; i++) {
		if (test2(passData, i) || test2(data, i)) {
			std::cout << Documents::V2::DataFieldCategoryAsString(static_cast<Documents::V2::DataFieldCategory>(i)) << "\n";
			test(passData, i);
			test(data, i);
			std::cout << "----------\n";
		}
		// }
	}
	*/
	// test_hsv(image);

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
