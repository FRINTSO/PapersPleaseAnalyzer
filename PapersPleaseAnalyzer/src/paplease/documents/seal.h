#pragma once
#include <opencv2/opencv.hpp>

#include "paplease/documents/doc_type.h"

namespace paplease {
	namespace documents {

		bool IsDocumentValidlySealed(const cv::Mat& mat, DocType documentType);

	}  // namespace documents
}  // namespace paplease
