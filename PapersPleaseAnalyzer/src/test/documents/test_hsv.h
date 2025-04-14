#pragma once
#include <opencv2/opencv.hpp>

#include "base/color.h"

namespace tests::documents
{

	void test_hsv(
		const cv::Mat& image,
		paplease::HSVConfig* hsvConfig = nullptr
	);

	cv::Mat find_hsv(
		const cv::Mat& image,
		paplease::HSVConfig& hsvConfig
	);

	cv::Mat find_hsl(
		const cv::Mat& image,
		paplease::HSLConfig& hslConfig
	);

}