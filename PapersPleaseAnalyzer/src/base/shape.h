#pragma once
#include "base/common.h"

namespace paplease {

	struct Shape
	{
		consteval Shape() = default;
		consteval Shape(int width, int height)
#if DOWNSCALE_OPTIMIZATION
			: width{ DOWNSCALE(width) }, height{ DOWNSCALE(height) }
		{}
#else
			: width{ width }, height{ height }
		{}
#endif

		int width, height;
	};

	struct Rectangle
	{
		constexpr Rectangle() = default;
		constexpr Rectangle(int x, int y, int width, int height)
#if DOWNSCALE_OPTIMIZATION
			: x{ DOWNSCALE(x) }, y{ DOWNSCALE(y) }, width{ DOWNSCALE(width) }, height{ DOWNSCALE(height) }
		{}
#else
			: x{ x }, y{ y }, width{ width }, height{ height }
		{}
#endif

		int x, y, width, height;

		constexpr bool Empty() const
		{
			return (width | height) == 0;
		}
	};

}  // namespace paplease
