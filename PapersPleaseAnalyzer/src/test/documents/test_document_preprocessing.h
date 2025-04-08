#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "base/documents/doc_type.h"

namespace paplease {
	namespace documents {
		namespace Test {

			void test_document_preprocessing(const std::string& number, documents::DocType docType);

		}
	}
}
