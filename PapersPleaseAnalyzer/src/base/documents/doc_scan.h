#pragma once
#include <opencv2/opencv.hpp>

#include "base/documents/doc_type.h"
#include "base/utils/fixed_array.h"

namespace paplease {
	namespace documents {

			struct ScannedDoc
			{
				cv::Rect boundingBox;
				documents::DocType documentType;
			};

			static inline constexpr size_t MaxDocumentsScanned = 4;
			using ScannedDocList = utils::FixedArray<ScannedDoc, MaxDocumentsScanned>;

			ScannedDocList ScanDocuments(const cv::Mat& binSearchArea);

	}  // namespace documents
}  // namespace paplease
