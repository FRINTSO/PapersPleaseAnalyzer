#pragma once
#include "paplease/common/scaled.h"

#include <opencv2/opencv.hpp>

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

	struct Point
	{
		constexpr Point()
			: x(0), y(0)
		{}

		constexpr Point(int x, int y)
			: x(x), y(y)
		{}

		constexpr bool operator!=(const Point& other) const noexcept
		{
			return x != other.x || y != other.y;
		}

		constexpr bool operator==(const Point& other) const noexcept
		{
			return !(*this != other);
		}

		int x, y;
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

		constexpr bool Intersects(const Rectangle& other) const noexcept
		{
			int a_x1 = x;
			int a_y1 = y;
			int a_x2 = x + width;
			int a_y2 = y + height;

			int b_x1 = other.x;
			int b_y1 = other.y;
			int b_x2 = other.x + other.width;
			int b_y2 = other.y + other.height;

			return a_x1 < b_x2 && a_x2 > b_x1 && a_y1 < b_y2 && a_y2 > b_y1;
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
