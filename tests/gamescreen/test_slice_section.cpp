/*
 * test_slice_section.cpp - Test game screen section slicing
 */

#include <opencv2/core/mat.hpp>
#include <paplease/game_screen.h>
#include "test.h"

/* Expected section coordinates from game_screen.cpp */
static constexpr int GAME_SCREEN_WIDTH = 1140;
static constexpr int GAME_SCREEN_HEIGHT = 670;
static constexpr int BOOTH_X = 0;
static constexpr int BOOTH_Y = 236;
static constexpr int BOOTH_WIDTH = 356;
static constexpr int BOOTH_HEIGHT = 434;
static constexpr int INSPECTION_X = 356;
static constexpr int INSPECTION_Y = 236;
static constexpr int INSPECTION_WIDTH = 784;
static constexpr int INSPECTION_HEIGHT = 434;

int main()
{
	/* Create a game screen with known pixel values */
	game_screen screen;
	screen.pixels = cv::Mat(GAME_SCREEN_HEIGHT, GAME_SCREEN_WIDTH, CV_8UC3);

	/* Fill entire screen with blue */
	screen.pixels.setTo(cv::Scalar(255, 0, 0)); /* BGR: Blue */

	/* Fill booth area with red */
	cv::Rect booth_rect(BOOTH_X, BOOTH_Y, BOOTH_WIDTH, BOOTH_HEIGHT);
	screen.pixels(booth_rect).setTo(cv::Scalar(0, 0, 255)); /* BGR: Red */

	/* Fill inspection area with green */
	cv::Rect insp_rect(INSPECTION_X, INSPECTION_Y, INSPECTION_WIDTH, INSPECTION_HEIGHT);
	screen.pixels(insp_rect).setTo(cv::Scalar(0, 255, 0)); /* BGR: Green */

	/* Test 1: Slice booth section */
	cv::Mat booth = slice_section(screen, ui_section::booth);
	TEST_ASSERT_EQ(booth.cols, BOOTH_WIDTH);
	TEST_ASSERT_EQ(booth.rows, BOOTH_HEIGHT);

	/* Booth should be red */
	cv::Vec3b booth_pixel = booth.at<cv::Vec3b>(0, 0);
	TEST_ASSERT_EQ(booth_pixel[0], 0);   /* B */
	TEST_ASSERT_EQ(booth_pixel[1], 0);   /* G */
	TEST_ASSERT_EQ(booth_pixel[2], 255); /* R */

	/* Test 2: Slice inspection section */
	cv::Mat inspection = slice_section(screen, ui_section::inspection);
	TEST_ASSERT_EQ(inspection.cols, INSPECTION_WIDTH);
	TEST_ASSERT_EQ(inspection.rows, INSPECTION_HEIGHT);

	/* Inspection should be green */
	cv::Vec3b insp_pixel = inspection.at<cv::Vec3b>(0, 0);
	TEST_ASSERT_EQ(insp_pixel[0], 0);   /* B */
	TEST_ASSERT_EQ(insp_pixel[1], 255); /* G */
	TEST_ASSERT_EQ(insp_pixel[2], 0);   /* R */

	/* Test 3: Verify booth and inspection don't overlap */
	TEST_ASSERT_EQ(BOOTH_X + BOOTH_WIDTH, INSPECTION_X);
	TEST_ASSERT_EQ(BOOTH_Y, INSPECTION_Y);
	TEST_ASSERT_EQ(BOOTH_HEIGHT, INSPECTION_HEIGHT);

	/* Test 4: Verify combined width matches expected game width for lower section */
	TEST_ASSERT_EQ(BOOTH_WIDTH + INSPECTION_WIDTH, GAME_SCREEN_WIDTH);

	return 0;
}

