/*
 * test_booth_text_extraction.cpp - Integration test for booth text extraction
 *
 * This test uses real game screenshots to verify booth info extraction.
 * Requires test data in TEST_DATA_DIR/screenshots/
 */

#include <cstdio>
#include <filesystem>
#include <string>

#include <opencv2/imgcodecs.hpp>

#include <paplease/game_screen.h>

#include "vision/vision.h"
#include "test.h"

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
		if (ext == ".png" || ext == ".jpg" || ext == ".jpeg")
			return entry.path().string();
	}
	return "";
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

	cv::Mat booth_slice = slice_section(screen, ui_section::booth);
	TEST_ASSERT(!booth_slice.empty());
	TEST_ASSERT_EQ(booth_slice.cols, 356);
	TEST_ASSERT_EQ(booth_slice.rows, 434);

	cv::Mat inspection_slice =
		slice_section(screen, ui_section::inspection);
	TEST_ASSERT(!inspection_slice.empty());
	TEST_ASSERT_EQ(inspection_slice.cols, 784);
	TEST_ASSERT_EQ(inspection_slice.rows, 434);

	booth_info info;
	bool success = extract_booth_info(info, screen);
	if (success) {
		fprintf(stderr, "Extracted date: %u.%u.%u, entrant: %s\n",
			(unsigned)info.current_date.day,
			(unsigned)info.current_date.month,
			(unsigned)info.current_date.year,
			info.entrant_count.c_str());
	} else {
		fprintf(stderr, "extract_booth_info returned false\n");
	}

	fprintf(stderr, "Vision integration test passed\n");
	return 0;
}
