/*
 * test_rgb_to_u32.cpp - Test rgb_color and bgr_color to_u32() conversion
 */

#include <paplease/colorspace.h>
#include "test.h"

int main()
{
	/* Pure red: 0xFF0000 */
	rgb_color red{255, 0, 0};
	TEST_ASSERT_EQ(red.to_u32(), 0xFF0000u);

	/* Pure green: 0x00FF00 */
	rgb_color green{0, 255, 0};
	TEST_ASSERT_EQ(green.to_u32(), 0x00FF00u);

	/* Pure blue: 0x0000FF */
	rgb_color blue{0, 0, 255};
	TEST_ASSERT_EQ(blue.to_u32(), 0x0000FFu);

	/* White: 0xFFFFFF */
	rgb_color white{255, 255, 255};
	TEST_ASSERT_EQ(white.to_u32(), 0xFFFFFFu);

	/* Black: 0x000000 */
	rgb_color black{0, 0, 0};
	TEST_ASSERT_EQ(black.to_u32(), 0x000000u);

	/* Arbitrary color */
	rgb_color arbitrary{0x12, 0x34, 0x56};
	TEST_ASSERT_EQ(arbitrary.to_u32(), 0x123456u);

	/* BGR to u32 should produce same output format (RGB packed) */
	bgr_color bgr_red{0, 0, 255}; /* BGR order: b=0, g=0, r=255 */
	TEST_ASSERT_EQ(bgr_red.to_u32(), 0xFF0000u);

	bgr_color bgr_blue{255, 0, 0}; /* BGR order: b=255, g=0, r=0 */
	TEST_ASSERT_EQ(bgr_blue.to_u32(), 0x0000FFu);

	/* Default constructor should give black */
	rgb_color default_color{};
	TEST_ASSERT_EQ(default_color.to_u32(), 0x000000u);

	return 0;
}

