#pragma once
#include <opencv2/opencv.hpp>

#include "base/documents_v2/doc_type.h"

namespace paplease {
	namespace documents {
		namespace v2 {

			bool IsDocumentValidlySealed(const cv::Mat& mat, DocType documentType);

		}  // namespace v2
	}  // namespace documents
}  // namespace paplease
