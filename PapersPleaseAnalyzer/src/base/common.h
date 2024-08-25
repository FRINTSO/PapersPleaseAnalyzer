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

consteval int Scale(int x) {
	if constexpr (IS_DOWNSCALED) {
		return static_cast<int>(x / SCALE);
	}
	else {
		return x;
	}
}

#define DOWNSCALE(x) Scale(x)
#endif
#undef PREFER_MACRO
