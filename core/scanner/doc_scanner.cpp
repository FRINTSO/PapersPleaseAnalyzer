#include "paplease/resources.h"
#include <array>

#include <opencv2/core.hpp>
#include <opencv2/core/types.hpp>

#include <paplease/geometry.h>
#include <paplease/compiler.h>
#include <paplease/documents.h>
#include <paplease/game_screen.h>
#include <paplease/scanner.h>
#include <paplease/colors.h>
#include <paplease/image.h>
#include <paplease/filters.h>

#include "bounding_box.h"

static cv::Mat create_minimized_doc_mask(const cv::Mat &booth_image)
{
	cv::Mat yellow_mask =
		to_hsv_binary(booth_image, filters::MINIMIZED_DOC_YELLOW);
	cv::Mat brown_mask =
		to_hsv_binary(booth_image, filters::MINIMIZED_DOC_BROWN);

	cv::Mat combined_mask;
	cv::bitwise_or(yellow_mask, brown_mask, combined_mask);
	return combined_mask;
}

static cv::Mat preprocess_booth_section(const cv::Mat &booth)
{
	const cv::Mat &doc_area_mask = resources::get_booth_doc_area_mask();

	// Create mask for minimized documents
	cv::Mat doc_mask = create_minimized_doc_mask(booth);

	// Apply document area constraint
	cv::Mat constrained_mask;
	cv::bitwise_or(doc_mask, doc_mask, constrained_mask, doc_area_mask);

	// Invert mask to select non-document regions
	cv::Mat inverted_mask;
	cv::bitwise_not(constrained_mask, inverted_mask);

	// Apply inverted mask to original image
	cv::Mat masked_booth;
	cv::bitwise_and(booth, booth, masked_booth, inverted_mask);

	// Final constraint to document area
	cv::Mat result;
	cv::bitwise_and(masked_booth, masked_booth, result, doc_area_mask);

	return result;
}

static cv::Mat preprocess_inspection_section(const cv::Mat &inspection)
{
	cv::Mat foreground_mask =
		to_hsl_binary(inspection, filters::INSPECTION_FOREGROUND);

	cv::Mat result;
	cv::bitwise_and(inspection, inspection, result, foreground_mask);
	return result;
}

static cv::Mat preprocess_section(const game_screen &screen, ui_section section)
{
	cv::Mat img = slice_section(screen, section);
	switch (section) {
	case ui_section::booth:
		return preprocess_booth_section(img);
	case ui_section::inspection:
		return preprocess_inspection_section(img);
	}
	unreachable();
}

static consteval std::array<rgb_color, 7> get_passport_colors()
{
	return { {
		get_appearance(doc_variant::passport_antegria).border_colors[0],
		get_appearance(doc_variant::passport_arstotzka).border_colors[0],
		get_appearance(doc_variant::passport_impor).border_colors[0],
		get_appearance(doc_variant::passport_kolechia).border_colors[0],
		get_appearance(doc_variant::passport_obristan).border_colors[0],
		get_appearance(doc_variant::passport_republia).border_colors[0],
		get_appearance(doc_variant::passport_united_federation)
			.border_colors[0],
	} };
}
static bool resolve_variant(doc_variant &out, const cv::Mat &img, doc_type type)
{
	if (type != doc_type::passport) {
		out = static_cast<doc_variant>(type);
		return true;
	}

	static constexpr auto passport_colors = get_passport_colors();

	for (int row = 0; row < img.rows; row++) {
		const bgr_color *pixels = img.ptr<bgr_color>(row);
		for (int col = 0; col < img.cols; col++) {
			u32 bgr = pixels[col].to_u32();

			for (size_t i = 0; i < passport_colors.size(); i++) {
				if (passport_colors[i].to_u32() == bgr) {
					out = static_cast<doc_variant>(
						static_cast<u8>(
							doc_variant::
								passport_antegria) +
						i);
					return true;
				}
			}
		}
	}
	return false;
}

bool find_doc(doc &out, const game_screen &screen, ui_section section,
	      doc_type type)
{
	cv::Mat processed = preprocess_section(screen, section);
	doc_variant variant;
	if (!resolve_variant(variant, processed, type))
		return false;

	cv::Rect box = find_bounding_box(processed, variant);
	if (box.empty())
		return false;

	out = { screen.pixels(box), variant };
	return true;
}
