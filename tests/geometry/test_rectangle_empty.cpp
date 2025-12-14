/*
 * test_rectangle_empty.cpp - Test rectangle::empty()
 *
 * Note: empty() returns true only when ALL fields (x, y, width, height) are zero.
 * This is a strict "default/uninitialized" check, not a "has no area" check.
 */

#include <paplease/geometry.h>
#include "test.h"

int main()
{
	/* A zero-initialized rectangle should be empty */
	rectangle empty_rect{0, 0, 0, 0};
	TEST_ASSERT(empty_rect.empty());

	/* Rectangle with position but no size is NOT empty (x,y are non-zero) */
	rectangle positioned{100, 200, 0, 0};
	TEST_ASSERT(!positioned.empty());

	/* Rectangle with actual dimensions is not empty */
	rectangle non_empty{10, 20, 100, 50};
	TEST_ASSERT(!non_empty.empty());

	/* Rectangle with only width is not empty */
	rectangle width_only{0, 0, 10, 0};
	TEST_ASSERT(!width_only.empty());

	/* Rectangle with only height is not empty */
	rectangle height_only{0, 0, 0, 10};
	TEST_ASSERT(!height_only.empty());

	/* Rectangle at origin with size is not empty */
	rectangle origin_with_size{0, 0, 50, 50};
	TEST_ASSERT(!origin_with_size.empty());

	return 0;
}

