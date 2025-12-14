/*
 * test_bgr_to_hsv_mask.cpp - Test bgr_to_hsv_mask() function
 */

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>
#include <paplease/colorspace.h>
#include "test.h"

int main()
{
	/* Create a simple 10x10 BGR image with solid red color */
	/* OpenCV BGR: Blue=0, Green=0, Red=255 -> pure red */
	cv::Mat red_image(10, 10, CV_8UC3, cv::Scalar(0, 0, 255));

	/* Red in HSV is approximately H=0, S=255, V=255 */
	/* OpenCV uses H: 0-179, S: 0-255, V: 0-255 */
	hsv_range red_range{
		.hue_min = 0,
		.hue_max = 10,
		.sat_min = 200,
		.sat_max = 255,
		.val_min = 200,
		.val_max = 255
	};

	cv::Mat mask = bgr_to_hsv_mask(red_image, red_range);

	/* Mask should be all white (255) since entire image is red */
	TEST_ASSERT_EQ(mask.rows, 10);
	TEST_ASSERT_EQ(mask.cols, 10);
	TEST_ASSERT_EQ(mask.type(), CV_8UC1);

	/* Check that all pixels are white (255 = in range) */
	int non_white_count = 0;
	for (int y = 0; y < mask.rows; y++) {
		for (int x = 0; x < mask.cols; x++) {
			if (mask.at<uchar>(y, x) != 255)
				non_white_count++;
		}
	}
	TEST_ASSERT_EQ(non_white_count, 0);

	/* Test with a blue image - should NOT match red HSV range */
	cv::Mat blue_image(10, 10, CV_8UC3, cv::Scalar(255, 0, 0)); /* BGR: Blue */
	cv::Mat blue_mask = bgr_to_hsv_mask(blue_image, red_range);

	/* All pixels should be black (0 = out of range) */
	int non_black_count = 0;
	for (int y = 0; y < blue_mask.rows; y++) {
		for (int x = 0; x < blue_mask.cols; x++) {
			if (blue_mask.at<uchar>(y, x) != 0)
				non_black_count++;
		}
	}
	TEST_ASSERT_EQ(non_black_count, 0);

	/* Test with mixed image - half red, half blue */
	cv::Mat mixed_image(10, 10, CV_8UC3);
	mixed_image(cv::Rect(0, 0, 5, 10)).setTo(cv::Scalar(0, 0, 255));  /* Left half red */
	mixed_image(cv::Rect(5, 0, 5, 10)).setTo(cv::Scalar(255, 0, 0)); /* Right half blue */

	cv::Mat mixed_mask = bgr_to_hsv_mask(mixed_image, red_range);

	/* Count white pixels - should be approximately half (50 pixels) */
	int white_count = 0;
	for (int y = 0; y < mixed_mask.rows; y++) {
		for (int x = 0; x < mixed_mask.cols; x++) {
			if (mixed_mask.at<uchar>(y, x) == 255)
				white_count++;
		}
	}
	TEST_ASSERT_EQ(white_count, 50); /* Left half = 5 * 10 = 50 */

	return 0;
}

