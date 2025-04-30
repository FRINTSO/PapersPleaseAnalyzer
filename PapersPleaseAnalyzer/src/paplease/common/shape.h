#pragma once
#include <opencv2/opencv.hpp>

#include "paplease/common/scaled.h"

namespace paplease {

	struct Shape
	{
		constexpr Shape()
			: width(0), height(0)
		{}

		constexpr Shape(int width, int height)
			: width(width), height(height)
		{}

		int width, height;
	};

	struct ScaledShape : public Shape
	{
		constexpr ScaledShape()
			: Shape()
		{}

		constexpr ScaledShape(scaled<int> width, scaled<int> height)
			: Shape(static_cast<int>(width), static_cast<int>(height))
		{}
	};

	struct Rectangle
	{

		constexpr Rectangle()
			: x(0), y(0), width(0), height(0)
		{}
		constexpr Rectangle(int x, int y, int width, int height)
			: x(x), y(y), width(width), height(height)
		{}

		constexpr bool Empty() const
		{
			return (width | height) == 0;
		}

		operator cv::Rect() const
		{
			return cv::Rect(x, y, width, height);
		}

		int x, y, width, height;
	};

	struct ScaledRectangle : public Rectangle
	{
		constexpr ScaledRectangle()
			: Rectangle()
		{}
		constexpr ScaledRectangle(scaled<int> x, scaled<int> y, scaled<int> width, scaled<int> height)
			: Rectangle(static_cast<int>(x), static_cast<int>(y), static_cast<int>(width), static_cast<int>(height))
		{}
	};

}  // namespace paplease
