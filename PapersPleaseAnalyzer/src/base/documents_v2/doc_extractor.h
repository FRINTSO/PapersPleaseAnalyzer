#pragma once
#include <optional>

#include <opencv2/opencv.hpp>

#include "base/shape.h"
#include "base/documents_v2/doc_appearance.h"

namespace paplease {
	namespace documents {
		namespace v2 {

			std::optional<cv::Mat> ExtractDocument(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance);

		}  // namespace v2
	}  // namespace documents
}  // namespace paplease
