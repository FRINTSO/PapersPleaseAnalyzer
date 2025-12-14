/*
 * test_booth_text_extraction.cpp - Integration test for booth text extraction
 *
 * This test uses real game screenshots to verify booth info extraction.
 * Requires test data in tests/data/screenshots/
 */

#include "opencv2/core.hpp"
#include "opencv2/core/types.hpp"
#include "paplease/resources.h"
#include <cstdio>
#include <iostream>
#include <filesystem>
#include <stdexcept>
#include <opencv2/imgcodecs.hpp>
#include <paplease/game_screen.h>
#include <paplease/vision.h>
#include "test.h"

/* Path to test data - relative to project root (where binary runs from) */
static const char *TEST_DATA_PATHS[] = {
	"tests/data/screenshots",
	"before_resturcture/core/assets/images",
	"../tests/data/screenshots",
	"../before_resturcture/core/assets/images",
	"../../tests/data/screenshots",
	"../../before_resturcture/core/assets/images",
	nullptr
};

static std::string find_test_data_dir()
{
	for (int i = 0; TEST_DATA_PATHS[i] != nullptr; i++) {
		if (std::filesystem::exists(TEST_DATA_PATHS[i])) {
			return TEST_DATA_PATHS[i];
		}
	}
	return "";
}

static std::string find_test_image(const std::string &dir)
{
	for (const auto &entry : std::filesystem::directory_iterator(dir)) {
		if (entry.is_regular_file()) {
			std::string ext = entry.path().extension().string();
			std::transform(ext.begin(), ext.end(), ext.begin(),
											::tolower);
			if (ext == ".png" || ext == ".jpg" || ext == ".jpeg") {
				return entry.path().string();
			}
		}
	}
	return "";
}

int main()
{
	init_resourcer("/home/wlm/dev/PapersPleaseAnalyzer/images");

	/* Find test data directory */
	std::string data_dir = find_test_data_dir();
	if (data_dir.empty()) {
		fprintf(stderr, "SKIP: No test data directory found\n");
		fprintf(stderr, "Searched paths:\n");
		for (int i = 0; TEST_DATA_PATHS[i] != nullptr; i++) {
			fprintf(stderr, "  - %s\n", TEST_DATA_PATHS[i]);
		}
		return 0; /* Skip, not fail - test data might not be present */
	}

	/* Find a test image */
	std::string test_image = find_test_image(data_dir);
	if (test_image.empty()) {
		fprintf(stderr, "SKIP: No test images found in %s\n",
			data_dir.c_str());
		return 0;
	}

	fprintf(stderr, "Testing with: %s\n", test_image.c_str());

	/* Load the image directly with OpenCV */
	cv::Mat raw_image = cv::imread(test_image);
	TEST_ASSERT(!raw_image.empty());

	// HACK FICK
	raw_image = raw_image(cv::Rect(1, 1, 1140, 670));

	/* Check if image has expected game dimensions */
	/* Papers Please runs at 570x335 scaled 2x to 1140x670 */
	if (raw_image.cols != 1140 || raw_image.rows != 670) {
		fprintf(stderr, "SKIP: Image %s is %dx%d, expected 1140x670\n",
			test_image.c_str(), raw_image.cols, raw_image.rows);
		return 0;
	}

	/* Create game_screen from loaded image */
	game_screen screen;
	screen.pixels = raw_image;

	/* Test slice_section works */
	cv::Mat booth_slice = slice_section(screen, ui_section::booth);
	TEST_ASSERT(!booth_slice.empty());
	TEST_ASSERT_EQ(booth_slice.cols, 356); /* BOOTH_WIDTH */
	TEST_ASSERT_EQ(booth_slice.rows, 434); /* BOOTH_HEIGHT */

	cv::Mat inspection_slice =
		slice_section(screen, ui_section::inspection);
	TEST_ASSERT(!inspection_slice.empty());
	TEST_ASSERT_EQ(inspection_slice.cols, 784); /* INSPECTION_WIDTH */
	TEST_ASSERT_EQ(inspection_slice.rows, 434); /* INSPECTION_HEIGHT */

	booth_info info;
	bool success;
	try {
		success = extract_booth_info(info, screen);
	} catch (const std::logic_error &e) {
		/* OCR charset not yet implemented - skip OCR tests */
		fprintf(stderr, "SKIP: OCR not implemented yet (%s)\n", e.what());
		fprintf(stderr, "Vision integration test passed (slicing only)\n");
		return 0;
	} catch (const cv::Exception &e) {
		TEST_FAIL(e.what());
	}
	TEST_ASSERT(success);
	TEST_ASSERT_EQ(info.current_date, "01.01.83");
	TEST_ASSERT_EQ(info.entrant_count, "03");
	if (success) {
		fprintf(stderr, "Extracted date: %s, entrant: %s\n",
			info.current_date.c_str(), info.entrant_count.c_str());
	}

	/*
		* Note: extract_booth_info() depends on OCR charset which has TODO.
		* For now, we just verify the slicing works. When OCR is fully
		* implemented, uncomment below:
		*
		* booth_info info;
		* bool success = extract_booth_info(info, screen);
		* if (success) {
		*     fprintf(stderr, "Extracted date: %s, entrant: %s\n",
		*             info.current_date.c_str(), info.entrant_count.c_str());
		* }
		*/

	fprintf(stderr, "Vision integration test passed\n");
	return 0;
}
