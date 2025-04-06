#pragma once

namespace paplease {

	struct Shape
	{
		int width, height;
	};

	struct Rectangle
	{
		int x, y, width, height;

		constexpr bool Empty() const
		{
			return (width | height) == 0;
		}
	};

}  // namespace paplease
