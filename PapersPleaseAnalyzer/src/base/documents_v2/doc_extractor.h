#pragma once
#include <opencv2/opencv.hpp>

#include "base/shape.h"
#include "base/documents_v2/doc_appearance.h"

namespace Documents::V2 {

	cv::Mat ExtractDocument(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance);

}