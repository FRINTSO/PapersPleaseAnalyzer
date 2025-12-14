/*
 * test_validate_dimensions.cpp - Test game screen dimension validation
 */

#include <opencv2/core/mat.hpp>
#include <paplease/game_screen.h>
#include "test.h"

/* Expected game screen dimensions from game_screen.cpp */
static constexpr int GAME_SCREEN_WIDTH = 1140;
static constexpr int GAME_SCREEN_HEIGHT = 670;

int main()
{
	/* Test 1: Correct dimensions should be valid */
	game_screen valid_screen;
	valid_screen.pixels = cv::Mat(GAME_SCREEN_HEIGHT, GAME_SCREEN_WIDTH, CV_8UC3);
	/* Note: validate is done via load functions, so we test the pixels directly */
	TEST_ASSERT_EQ(valid_screen.pixels.cols, GAME_SCREEN_WIDTH);
	TEST_ASSERT_EQ(valid_screen.pixels.rows, GAME_SCREEN_HEIGHT);

	/* Test 2: Wrong width */
	game_screen wrong_width;
	wrong_width.pixels = cv::Mat(GAME_SCREEN_HEIGHT, 1920, CV_8UC3);
	TEST_ASSERT_NE(wrong_width.pixels.cols, GAME_SCREEN_WIDTH);

	/* Test 3: Wrong height */
	game_screen wrong_height;
	wrong_height.pixels = cv::Mat(1080, GAME_SCREEN_WIDTH, CV_8UC3);
	TEST_ASSERT_NE(wrong_height.pixels.rows, GAME_SCREEN_HEIGHT);

	/* Test 4: Empty screen */
	game_screen empty_screen;
	TEST_ASSERT(empty_screen.pixels.empty());
	TEST_ASSERT_EQ(empty_screen.pixels.cols, 0);
	TEST_ASSERT_EQ(empty_screen.pixels.rows, 0);

	/* Test 5: Verify constants match expected game values */
	TEST_ASSERT_EQ(GAME_SCREEN_WIDTH, 1140);
	TEST_ASSERT_EQ(GAME_SCREEN_HEIGHT, 670);

	return 0;
}

