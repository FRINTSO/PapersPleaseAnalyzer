#include "opencv2/core.hpp"
#include <opencv2/core/mat.hpp>

#include <paplease/compiler.h>
#include <paplease/game_screen.h>
#include <paplease/geometry.h>
#include <paplease/vision.h>
#include <paplease/colorspace.h>
#include <paplease/ocr.h>

static constexpr hsv_range BOOTH_TEXT_EXTRACTION_MASK_HSV{ .hue_min = 12,
						   .hue_max = 18,
						   .sat_min = 0,
						   .sat_max = 255,
						   .val_min = 106,
						   .val_max = 106 };

static constexpr rectangle BOOTH_TEXT_RECT{ .x = 168,
					    .y = 416,
					    .width = 22,
					    .height = 12 };

static constexpr rectangle BOOTH_CURRENT_DATE_RECT{ .x = 8,
						    .y = 418,
						    .width = 56,
						    .height = 12 };
static constexpr rectangle BOOTH_ENTRACT_COUNTER_RECT{ .x = 168,
						       .y = 416,
						       .width = 22,
						       .height = 12 };

static cv::Mat preprocess_booth_for_text_extraction(const cv::Mat &booth_image)
{
	cv::Mat mask =
		bgr_to_hsv_mask(booth_image, BOOTH_TEXT_EXTRACTION_MASK_HSV);
	cv::Mat unflipped_region = mask(BOOTH_TEXT_RECT.to_cv());
	// unflipped_region ^= 255;
	cv::bitwise_not(unflipped_region, unflipped_region);
	return mask;
}


bool extract_booth_info(booth_info &out, const game_screen &screen,
			const resources_ctx &ctx)
{
	cv::Mat booth = slice_section(screen, ui_section::booth);
	cv::Mat processed_booth = preprocess_booth_for_text_extraction(booth);

	cv::Mat current_date_region =
		processed_booth(BOOTH_CURRENT_DATE_RECT.to_cv());
	std::string current_date;
	if (!extract_text_strict(current_date, current_date_region,
				 typeface::booth, ctx))
		return false;

	cv::Mat entrant_count_region =
		processed_booth(BOOTH_ENTRACT_COUNTER_RECT.to_cv());
	std::string entrant_count;
	if (!extract_text_strict(entrant_count, entrant_count_region,
				 typeface::booth, ctx))
		return false;

	if (current_date.empty() || entrant_count.empty())
		return false;

	if (!parse_date(out.current_date, current_date))
		return false;

	out.entrant_count = entrant_count;
	return true;
}
