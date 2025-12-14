#ifndef PAPLEASE_GEOMETRY_H
#define PAPLEASE_GEOMETRY_H

#include <opencv2/core/types.hpp>

struct point {
	int x, y;
};

struct shape {
	int width, height;
};

struct rectangle {
	int x, y, width, height;

	bool empty() const noexcept
	{
		return x == 0 && y == 0 && width == 0 && height == 0;
	}

	cv::Rect to_cv() const
	{
		return { x, y, width, height };
	}
};

#endif // PAPLEASE_GEOMETRY_H
