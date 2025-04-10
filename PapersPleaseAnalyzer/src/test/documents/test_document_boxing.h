#pragma once
#include <string>

#include "base/documents/doc_type.h"
#include "base/documents/doc_data.h"

namespace paplease {
	namespace documents {

		namespace test {

			void test_document_character_boxing(const std::string& number, DocType docType);
			void test_document_field_boxing(const std::string& number, DocType docType);
			void test_text_field_character_boxing(const std::string& number, DocType docType, FieldCategory dataField);

		}
	}
}