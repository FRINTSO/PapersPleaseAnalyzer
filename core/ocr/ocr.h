#ifndef PAPLEASE_OCR_H
#define PAPLEASE_OCR_H
#include <string>
#include <vector>

#include <opencv2/core/mat.hpp>

#include <paplease/geometry.h>
#include <paplease/documents.h>

enum class typeface {
	t04b03,
	bm_mini,
	mini_kylie,
	booth,
};

struct font_info;

u64 encode_character_bits(const cv::Mat &character);
std::vector<rectangle> extract_character_boxes(const cv::Mat &image, const font_info &font);
bool extract_text_strict(std::string &out, const cv::Mat &binary_image, typeface tf);

constexpr typeface typeface_for(doc_type type)
{
	switch (type) {
	case doc_type::access_permit:
	case doc_type::certificate_of_vaccination:
	case doc_type::diplomatic_authorization:
	case doc_type::entry_permit:
	case doc_type::entry_ticket:
	case doc_type::grant_of_asylum:
	case doc_type::identity_supplement:
	case doc_type::work_pass:
	case doc_type::rulebook:
	case doc_type::bulletin:
	case doc_type::passport:
		return typeface::bm_mini;
	case doc_type::identity_card:
		return typeface::mini_kylie;
	case doc_type::transcript:
		return typeface::t04b03;
	}
	unreachable();
}

#endif // PAPLEASE_OCR_H
