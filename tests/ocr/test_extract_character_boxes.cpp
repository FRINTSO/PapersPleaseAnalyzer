/*
 * test_extract_character_boxes.cpp - Test character box extraction for OCR
 *
 * Tests the algorithm that finds bounding boxes around individual characters
 * in a binary image.
 */

#include <cstdio>
#include <vector>
#include <opencv2/core/mat.hpp>
#include <paplease/geometry.h>
#include <paplease/types.h>
#include "test.h"

/*
 * Local copy of font_info and extraction logic from ocr.cpp for testing.
 */
struct font_info {
	int max_pixels_tall;
	int letter_spacing_horizontal;
	int whitespace_pixel_width;
	int letter_spacing_vertical;
	int newline_size;
};

static int find_top_line(const cv::Mat &mat)
{
	for (int row = 0; row < mat.rows; row++) {
		for (int col = 0; col < mat.cols; col++) {
			if (mat.at<uchar>(row, col))
				continue;
			return row;
		}
	}
	return -1;
}

static int find_next_top_line(const cv::Mat &mat, int previous_line, int font_size)
{
	for (int row = previous_line + font_size; row < mat.rows; row++) {
		for (int col = 0; col < mat.cols; col++) {
			if (mat.at<uchar>(row, col))
				continue;
			return row;
		}
	}
	return -1;
}

static std::vector<rectangle> extract_character_boxes(const cv::Mat &image,
						      const font_info &font)
{
	std::vector<rectangle> boxes{};
	constexpr int max_allowed_whitespace = 10;

	const int whitespace_size = font.whitespace_pixel_width;
	const int max_steps_from_last_character =
		whitespace_size * max_allowed_whitespace;

	int last_box_x = INT_MAX;

	int left = INT_MAX;
	int top = INT_MAX;
	int right = 0;
	int bottom = 0;
	for (int x = 0; x < image.cols; x++) {
		if (x - last_box_x > max_steps_from_last_character)
			break;

		bool column_has_black_pixel = false;
		for (int y = 0; y < image.rows; y++) {
			if (image.at<uchar>(y, x))
				continue;
			column_has_black_pixel = true;
			top = std::min(top, y);
			bottom = std::max(bottom, y);
		}

		if (column_has_black_pixel) {
			left = std::min(left, x);
			right = std::max(right, x);
		} else if (left != INT_MAX) {
			last_box_x = x;

			int width = right - left + 1;
			int height = bottom - top + 1;
			boxes.emplace_back(rectangle{left, top, width, height});
			left = INT_MAX;
			top = INT_MAX;
			right = 0;
			bottom = 0;
		}
	}

	return boxes;
}

int main()
{
	font_info test_font{
		.max_pixels_tall = 12,
		.letter_spacing_horizontal = 4,
		.whitespace_pixel_width = 6,
		.letter_spacing_vertical = 8,
		.newline_size = 4
	};

	/* Test 1: Empty image (all white) - should return no boxes */
	TEST_LOG("Test 1: Empty image (all white)");
	cv::Mat empty_image(20, 100, CV_8UC1, cv::Scalar(255));
	std::vector<rectangle> empty_boxes = extract_character_boxes(empty_image, test_font);
	TEST_LOG("  Found %zu boxes (expected 0)", empty_boxes.size());
	TEST_ASSERT_EQ(empty_boxes.size(), 0u);

	/* Test 2: Single character (3x5 black rectangle) */
	TEST_LOG("Test 2: Single character (3x5 black rectangle at x=10, y=5)");
	cv::Mat single_char(20, 100, CV_8UC1, cv::Scalar(255));
	/* Draw a 3-wide, 5-tall black rectangle at x=10, y=5 */
	for (int y = 5; y < 10; y++) {
		for (int x = 10; x < 13; x++) {
			single_char.at<uchar>(y, x) = 0;
		}
	}
	std::vector<rectangle> single_boxes = extract_character_boxes(single_char, test_font);
	TEST_LOG("  Found %zu boxes", single_boxes.size());
	if (single_boxes.size() > 0) {
		TEST_LOG("  Box 0: x=%d y=%d w=%d h=%d",
			single_boxes[0].x, single_boxes[0].y,
			single_boxes[0].width, single_boxes[0].height);
	}
	TEST_ASSERT_EQ(single_boxes.size(), 1u);
	TEST_ASSERT_EQ(single_boxes[0].x, 10);
	TEST_ASSERT_EQ(single_boxes[0].y, 5);
	TEST_ASSERT_EQ(single_boxes[0].width, 3);
	TEST_ASSERT_EQ(single_boxes[0].height, 5);

	/* Test 3: Two separated characters */
	TEST_LOG("Test 3: Two separated characters");
	cv::Mat two_chars(20, 100, CV_8UC1, cv::Scalar(255));
	/* First char at x=5 */
	for (int y = 2; y < 8; y++) {
		for (int x = 5; x < 9; x++) {
			two_chars.at<uchar>(y, x) = 0;
		}
	}
	/* Second char at x=20 (gap of 11 pixels) */
	for (int y = 3; y < 9; y++) {
		for (int x = 20; x < 25; x++) {
			two_chars.at<uchar>(y, x) = 0;
		}
	}
	std::vector<rectangle> two_boxes = extract_character_boxes(two_chars, test_font);
	TEST_LOG("  Found %zu boxes", two_boxes.size());
	for (size_t i = 0; i < two_boxes.size(); i++) {
		TEST_LOG("  Box %zu: x=%d y=%d w=%d h=%d",
			i, two_boxes[i].x, two_boxes[i].y,
			two_boxes[i].width, two_boxes[i].height);
	}
	TEST_ASSERT_EQ(two_boxes.size(), 2u);

	/* First box */
	TEST_ASSERT_EQ(two_boxes[0].x, 5);
	TEST_ASSERT_EQ(two_boxes[0].y, 2);
	TEST_ASSERT_EQ(two_boxes[0].width, 4);
	TEST_ASSERT_EQ(two_boxes[0].height, 6);

	/* Second box */
	TEST_ASSERT_EQ(two_boxes[1].x, 20);
	TEST_ASSERT_EQ(two_boxes[1].y, 3);
	TEST_ASSERT_EQ(two_boxes[1].width, 5);
	TEST_ASSERT_EQ(two_boxes[1].height, 6);

	/* Test 4: find_top_line helper */
	TEST_LOG("Test 4: find_top_line helper");
	cv::Mat line_test(30, 50, CV_8UC1, cv::Scalar(255));
	/* Put black pixel at row 7 */
	line_test.at<uchar>(7, 10) = 0;
	int top_line = find_top_line(line_test);
	TEST_LOG("  Found top line at row %d (expected 7)", top_line);
	TEST_ASSERT_EQ(top_line, 7);

	/* Test 5: find_top_line on empty image */
	TEST_LOG("Test 5: find_top_line on empty image");
	cv::Mat all_white(30, 50, CV_8UC1, cv::Scalar(255));
	int no_line = find_top_line(all_white);
	TEST_LOG("  Found top line at row %d (expected -1)", no_line);
	TEST_ASSERT_EQ(no_line, -1);

	/* Test 6: find_next_top_line */
	TEST_LOG("Test 6: find_next_top_line");
	cv::Mat multi_line(60, 50, CV_8UC1, cv::Scalar(255));
	multi_line.at<uchar>(5, 10) = 0;  /* First line at row 5 */
	multi_line.at<uchar>(25, 10) = 0; /* Second line at row 25 */
	int first_line = find_top_line(multi_line);
	TEST_LOG("  First line at row %d (expected 5)", first_line);
	TEST_ASSERT_EQ(first_line, 5);
	int second_line = find_next_top_line(multi_line, first_line, 12);
	TEST_LOG("  Second line at row %d (expected 25)", second_line);
	TEST_ASSERT_EQ(second_line, 25);

	TEST_LOG("All tests passed!");
	return 0;
}

