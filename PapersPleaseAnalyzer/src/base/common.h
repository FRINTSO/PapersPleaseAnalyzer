#pragma once
#include <opencv2/opencv.hpp>

#define PREFER_MACRO false
#if PREFER_MACRO
#define IS_DOWNSCALED false
#define SCALE 0.25f

#if IS_DOWNSCALED
#define DOWNSCALE(x) (int)(x/SCALE)
#else
#define DOWNSCALE(x) x
#endif
#else
constexpr bool IS_DOWNSCALED = false;
constexpr float SCALE = 0.25f;

consteval int Downscale(int x) {
	if constexpr (IS_DOWNSCALED) {
		return static_cast<int>(x / SCALE);
	}
	else {
		return x;
	}
}

#define DOWNSCALE(x) Downscale(x)
#endif
#undef PREFER_MACRO

inline cv::Mat DownScaleImage(cv::Mat in, float scale) {
	cv::Mat result;
	cv::resize(in, result, cv::Size((int)((float)in.cols / scale), (int)((float)in.rows / scale)), 0, 0, cv::INTER_NEAREST);
	return result;
}