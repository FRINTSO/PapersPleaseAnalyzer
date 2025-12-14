#ifndef PAPLEASE_OCR_H
#define PAPLEASE_OCR_H
#include <string>

#include <opencv2/core/mat.hpp>

enum class typeface {
	t04b03,
	bm_mini,
	mini_kylie,
	booth,
};

bool extract_text_strict(std::string &out, const cv::Mat &binary_image, typeface tf);

#endif // PAPLEASE_OCR_H
