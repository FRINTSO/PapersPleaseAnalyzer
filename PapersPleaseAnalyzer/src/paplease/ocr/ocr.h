#pragma once
#include <string>
#include <vector>

#include <opencv2/core/mat.hpp>

#include "paplease/common/shape.h"
#include "paplease/ocr/font.h"

namespace paplease {
	namespace ocr {
		
		std::vector<Rectangle> ImageToBoxes(const cv::Mat& mat, const FontInfo& fontInfo);
		std::string ImageToString(const cv::Mat& mat, const FontInfo& fontInfo);

	}  // namespace ocr
}  // namespace paplease
