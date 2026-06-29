/*
 * test_booth_polarity.cpp - Verify booth text regions have correct polarity
 *
 * After preprocessing, all three booth text regions (date, counter, weight)
 * must have the OCR-expected polarity: text pixels = 0, background = 255.
 *
 * The character scanner (find_top_line, scan_line_for_boxes) treats 0 as
 * foreground and non-zero as background. If any region is inverted, OCR
 * produces garbage and extract_booth_info silently returns false.
 *
 * Requires a game screenshot in TEST_DATA_DIR/screenshots/
 */

#include <cstdio>
#include <filesystem>
#include <string>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <paplease/colorspace.h>
#include <paplease/game_screen.h>
#include <paplease/geometry.h>

#include "vision/colorspace.h"
#include "test.h"

static constexpr hsv_range BOOTH_TEXT_EXTRACTION_MASK_HSV{
	.hue_min = 12, .hue_max = 18,
	.sat_min = 0,  .sat_max = 255,
	.val_min = 106, .val_max = 106
};

static constexpr rectangle BOOTH_TEXT_RECT{ .x = 168, .y = 416,
					    .width = 22, .height = 12 };
static constexpr rectangle BOOTH_CURRENT_DATE_RECT{ .x = 8, .y = 418,
						    .width = 56, .height = 12 };
static constexpr rectangle BOOTH_ENTRACT_COUNTER_RECT{ .x = 168, .y = 416,
						       .width = 22, .height = 12 };
static constexpr rectangle BOOTH_ENTRANT_WEIGHT_RECT{ .x = 302, .y = 390,
						      .width = 44, .height = 12 };

static std::string find_test_image()
{
	std::string dir = std::string(TEST_DATA_DIR) + "/screenshots";
	if (!std::filesystem::exists(dir))
		return "";

	for (const auto &entry : std::filesystem::directory_iterator(dir)) {
		if (!entry.is_regular_file())
			continue;
		std::string ext = entry.path().extension().string();
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
		if (ext == ".png")
			return entry.path().string();
	}
	return "";
}

/*
 * Check that a region has correct polarity for the OCR scanner:
 * if there are any text pixels, they should be 0 (black).
 * Returns true if the region is all-white (no text, fine) or has
 * text pixels at value 0.
 */
static bool check_polarity(const cv::Mat &region, const char *name)
{
	int zero_count = 0;
	int nonzero_count = 0;

	for (int y = 0; y < region.rows; y++) {
		for (int x = 0; x < region.cols; x++) {
			if (region.at<uchar>(y, x) == 0)
				zero_count++;
			else
				nonzero_count++;
		}
	}

	int total = region.rows * region.cols;
	fprintf(stderr, "  %-10s: %d zero, %d nonzero (of %d)\n",
		name, zero_count, nonzero_count, total);

	/*
	 * If the region has text, there should be SOME zero pixels (text)
	 * and SOME nonzero pixels (background). If ALL pixels are zero,
	 * the polarity is likely inverted — the mask matched the background
	 * instead of just the text.
	 */
	if (zero_count == total) {
		fprintf(stderr, "    WARNING: all pixels are zero — "
			"possible inverted polarity\n");
		return false;
	}

	return true;
}

int main()
{
	std::string test_image = find_test_image();
	if (test_image.empty()) {
		fprintf(stderr, "SKIP: No test images in %s/screenshots\n",
			TEST_DATA_DIR);
		return 0;
	}

	fprintf(stderr, "Testing with: %s\n", test_image.c_str());

	game_screen screen;
	TEST_ASSERT(load_game_screen_from_file(screen, test_image));

	cv::Mat booth = slice_section(screen, ui_section::booth);
	TEST_ASSERT(!booth.empty());

	/* Replicate preprocess_booth_for_text_extraction */
	cv::Mat mask = bgr_to_hsv_mask(booth, BOOTH_TEXT_EXTRACTION_MASK_HSV);
	cv::Mat counter_region = mask(BOOTH_TEXT_RECT.to_cv());
	cv::bitwise_not(counter_region, counter_region);

	/* Extract the three regions */
	cv::Mat date_roi = mask(BOOTH_CURRENT_DATE_RECT.to_cv());
	cv::Mat count_roi = mask(BOOTH_ENTRACT_COUNTER_RECT.to_cv());
	cv::Mat weight_roi = mask(BOOTH_ENTRANT_WEIGHT_RECT.to_cv());

	fprintf(stderr, "\nPolarity check (text should be 0, bg should be 255):\n");

	bool date_ok = check_polarity(date_roi, "date");
	bool count_ok = check_polarity(count_roi, "counter");
	bool weight_ok = check_polarity(weight_roi, "weight");

	if (!date_ok || !count_ok || !weight_ok) {
		fprintf(stderr,
			"\nFAIL: One or more regions have bad polarity. "
			"The bitwise_not in preprocess_booth_for_text_extraction "
			"may need to cover all three regions, not just "
			"BOOTH_TEXT_RECT.\n");
		return 1;
	}

	fprintf(stderr, "\nBooth polarity test passed\n");
	return 0;
}
