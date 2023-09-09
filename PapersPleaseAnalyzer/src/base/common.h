#pragma once
#include <opencv2/opencv.hpp>

#define IS_DOWNSCALED false
#define SCALE 0.25f

#if IS_DOWNSCALED
#define DOWNSCALE(x) (int)(x/SCALE)
#else
#define DOWNSCALE(x) x
#endif

inline cv::Mat DownScale(cv::Mat in, float scale) {
	cv::Mat result;
	cv::resize(in, result, cv::Size((int)((float)in.cols / scale), (int)((float)in.rows / scale)), 0, 0, cv::INTER_NEAREST);
	return result;
}