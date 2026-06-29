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

#include "ocr/ocr.h"
#include "ocr/font_info.h"
#include "test.h"

int main()
{
	font_info test_font{
		.tf = typeface::booth,
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
	for (int y = 5; y < 10; y++)
		for (int x = 10; x < 13; x++)
			single_char.at<uchar>(y, x) = 0;

	std::vector<rectangle> single_boxes = extract_character_boxes(single_char, test_font);
	TEST_LOG("  Found %zu boxes", single_boxes.size());
	TEST_ASSERT_EQ(single_boxes.size(), 1u);
	TEST_ASSERT_EQ(single_boxes[0].x, 10);
	TEST_ASSERT_EQ(single_boxes[0].y, 5);
	TEST_ASSERT_EQ(single_boxes[0].width, 3);
	TEST_ASSERT_EQ(single_boxes[0].height, 5);

	/* Test 3: Two separated characters */
	TEST_LOG("Test 3: Two separated characters");
	cv::Mat two_chars(20, 100, CV_8UC1, cv::Scalar(255));
	for (int y = 2; y < 8; y++)
		for (int x = 5; x < 9; x++)
			two_chars.at<uchar>(y, x) = 0;
	for (int y = 3; y < 9; y++)
		for (int x = 20; x < 25; x++)
			two_chars.at<uchar>(y, x) = 0;

	std::vector<rectangle> two_boxes = extract_character_boxes(two_chars, test_font);
	TEST_LOG("  Found %zu boxes", two_boxes.size());
	TEST_ASSERT_EQ(two_boxes.size(), 2u);
	TEST_ASSERT_EQ(two_boxes[0].x, 5);
	TEST_ASSERT_EQ(two_boxes[0].y, 2);
	TEST_ASSERT_EQ(two_boxes[0].width, 4);
	TEST_ASSERT_EQ(two_boxes[0].height, 6);
	TEST_ASSERT_EQ(two_boxes[1].x, 20);
	TEST_ASSERT_EQ(two_boxes[1].y, 3);
	TEST_ASSERT_EQ(two_boxes[1].width, 5);
	TEST_ASSERT_EQ(two_boxes[1].height, 6);

	TEST_LOG("All tests passed!");
	return 0;
}
