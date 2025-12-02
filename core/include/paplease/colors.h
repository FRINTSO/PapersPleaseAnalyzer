#ifndef PAPLEASE_COLORS_H
#define PAPLEASE_COLORS_H

#include <opencv2/core/mat.hpp>

#include <paplease/integral_types.h>

struct rgb_color {
    u8 r, g, b;

    constexpr rgb_color() : r(0), g(0), b(0) {}
    constexpr rgb_color(u8 r, u8 g, u8 b) : r(r), g(g), b(b) {}

    constexpr u32 to_u32() const {
        return (static_cast<u32>(r) << 16) |
               (static_cast<u32>(g) << 8) |
               static_cast<u32>(b);
    }
};

struct bgr_color {
    u8 b, g, r;

    constexpr u32 to_u32() const {
        return (static_cast<u32>(r) << 16) |
               (static_cast<u32>(g) << 8) |
               static_cast<u32>(b);
    }
};
struct hsv_config {
	i32 hueMin = 0;
	i32 hueMax = 179;
	i32 satMin = 0;
	i32 satMax = 255;
	i32 valMin = 0;
	i32 valMax = 255;

	bool is_empty() const
	{
		return (hueMin | satMin | valMin) == 0 &&
		       (satMax & valMax) == 255 && (hueMax == 179);
	}
};

struct hsl_config {
	i32 hueMin = 0;
	i32 hueMax = 179;
	i32 satMin = 0;
	i32 satMax = 255;
	i32 litMin = 0;
	i32 litMax = 255;
};


cv::Mat to_hsv_binary(const cv::Mat &bgrMat, const hsv_config &hsvConfig);
cv::Mat to_hsl_binary(const cv::Mat &bgrMat, const hsl_config &hslConfig);
#endif // PAPLEASE_COLORS_H
