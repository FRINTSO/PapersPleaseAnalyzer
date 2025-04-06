#pragma once
#include <string>

#include "base/documents_v2/doc_type.h"
#include "base/documents_v2/doc_data.h"

namespace paplease {
	namespace documents {

		namespace v2::test {

			void test_document_character_boxing(const std::string& number, DocType docType);
			void test_document_field_boxing(const std::string& number, DocType docType);
			void test_text_field_character_boxing(const std::string& number, DocType docType, DataFieldCategory dataField);

		}
	}
}