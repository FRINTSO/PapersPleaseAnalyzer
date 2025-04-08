#pragma once
#include <optional>

#include <opencv2/opencv.hpp>

#include "base/shape.h"
#include "base/documents/doc_appearance.h"

namespace paplease {
	namespace documents {

		std::optional<cv::Mat> ExtractDocument(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance);

	}  // namespace documents
}  // namespace paplease
