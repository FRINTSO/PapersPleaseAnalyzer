#pragma once
#include <string>

#include "base/documents_v2/doc_type.h"
#include "base/documents_v2/doc_layout.h"

namespace Documents {

	namespace V1::Test {

		void test_access_permit_text_boxing(const std::string& number);
		void test_booth_text_boxing(const std::string& number);
		void test_certificate_of_vaccination_text_boxing(const std::string& number);
		void test_diplomatic_authorization_text_boxing(const std::string& number);
		void test_entry_permit_text_boxing(const std::string& number);
		void test_entry_ticket_text_boxing(const std::string& number);
		void test_grant_of_asylum_text_boxing(const std::string& number);
		void test_identity_card_text_boxing(const std::string& number);
		void test_identity_supplement_text_boxing(const std::string& number);
		void test_passport_text_boxing(const std::string& number);
		void test_work_pass_text_boxing(const std::string& number);

	}

	namespace V2::Test {
		void test_document_character_boxing(const std::string& number, DocType docType);
		void test_document_field_boxing(const std::string& number, DocType docType);
		void test_text_field_character_boxing(const std::string& number, DocType docType, DataFieldCategory dataField);
	}
}