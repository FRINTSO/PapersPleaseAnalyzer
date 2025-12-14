/*
 * test_rectangle_to_cv.cpp - Test rectangle::to_cv() conversion
 */

#include <opencv2/core/types.hpp>
#include <paplease/geometry.h>
#include "test.h"

int main()
{
	/* Basic conversion */
	rectangle rect{10, 20, 100, 50};
	cv::Rect cv_rect = rect.to_cv();

	TEST_ASSERT_EQ(cv_rect.x, 10);
	TEST_ASSERT_EQ(cv_rect.y, 20);
	TEST_ASSERT_EQ(cv_rect.width, 100);
	TEST_ASSERT_EQ(cv_rect.height, 50);

	/* Zero rectangle */
	rectangle zero_rect{0, 0, 0, 0};
	cv::Rect cv_zero = zero_rect.to_cv();

	TEST_ASSERT_EQ(cv_zero.x, 0);
	TEST_ASSERT_EQ(cv_zero.y, 0);
	TEST_ASSERT_EQ(cv_zero.width, 0);
	TEST_ASSERT_EQ(cv_zero.height, 0);

	/* Large values */
	rectangle large_rect{1920, 1080, 3840, 2160};
	cv::Rect cv_large = large_rect.to_cv();

	TEST_ASSERT_EQ(cv_large.x, 1920);
	TEST_ASSERT_EQ(cv_large.y, 1080);
	TEST_ASSERT_EQ(cv_large.width, 3840);
	TEST_ASSERT_EQ(cv_large.height, 2160);

	return 0;
}

