#ifndef PAPLEASE_OCR_H
#define PAPLEASE_OCR_H
#include <string>

#include <opencv2/core/mat.hpp>

#include <paplease/resources.h>

enum class typeface {
	t04b03,
	bm_mini,
	mini_kylie,
	booth,
};

// Encode a character glyph image into a u64 for charset lookup
u64 encode_character_bits(const cv::Mat &character);

bool extract_text_strict(std::string &out, const cv::Mat &binary_image, typeface tf, const resources_ctx& ctx);

#endif // PAPLEASE_OCR_H
