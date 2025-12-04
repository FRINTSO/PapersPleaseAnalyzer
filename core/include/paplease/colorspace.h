#ifndef PAPLEASE_COLORSPACE_H
#define PAPLEASE_COLORSPACE_H

#include <opencv2/core/mat.hpp>

#include <paplease/types.h>

struct rgb_color {
	u8 r, g, b;

	constexpr rgb_color()
		: r(0)
		, g(0)
		, b(0)
	{
	}
	constexpr rgb_color(u8 r, u8 g, u8 b)
		: r(r)
		, g(g)
		, b(b)
	{
	}

	constexpr u32 to_u32() const
	{
		return (static_cast<u32>(r) << 16) |
		       (static_cast<u32>(g) << 8) | static_cast<u32>(b);
	}
};

struct bgr_color {
	u8 b, g, r;

	constexpr u32 to_u32() const
	{
		return (static_cast<u32>(r) << 16) |
		       (static_cast<u32>(g) << 8) | static_cast<u32>(b);
	}
};

struct hsv_range {
	i32 hue_min = 0;
	i32 hue_max = 179;
	i32 sat_min = 0;
	i32 sat_max = 255;
	i32 val_min = 0;
	i32 val_max = 255;
};

struct hsl_range {
	i32 hue_min = 0;
	i32 hue_max = 179;
	i32 sat_min = 0;
	i32 sat_max = 255;
	i32 lit_min = 0;
	i32 lit_max = 255;
};

cv::Mat bgr_to_hsv_mask(const cv::Mat& bgr_image, const hsv_range& range);
cv::Mat bgr_to_hsl_mask(const cv::Mat& bgr_image, const hsl_range& range);

#endif // PAPLEASE_COLORSPACE_H
