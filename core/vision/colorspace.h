#ifndef PAPLEASE_CORE_VISION_COLORSPACE_H
#define PAPLEASE_CORE_VISION_COLORSPACE_H

#include <opencv2/core/mat.hpp>
#include <paplease/colorspace.h>

cv::Mat bgr_to_hsv_mask(const cv::Mat &bgr_image, const hsv_range &range);
cv::Mat bgr_to_hsl_mask(const cv::Mat &bgr_image, const hsl_range &range);

#endif
