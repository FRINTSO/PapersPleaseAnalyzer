#pragma once
#include "paplease/documents/doc_type.h"

#include <opencv2/opencv.hpp>

namespace paplease {
	namespace documents {

		bool HasSeal(DocType documentType);
		bool IsDocumentValidlySealed(const cv::Mat& mat, DocType documentType);

	}  // namespace documents
}  // namespace paplease
