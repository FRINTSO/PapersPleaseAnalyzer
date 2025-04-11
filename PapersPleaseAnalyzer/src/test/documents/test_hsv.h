#pragma once
#include <opencv2/opencv.hpp>

#include "base/image_process.h"

namespace tests::documents
{

	void test_hsv(
		const cv::Mat& image,
		int hueMin = 0,
		int hueMax = 179,
		int satMin = 0,
		int satMax = 127,
		int valMin = 0,
		int valMax = 255
	);

	cv::Mat find_hsv(
		const cv::Mat& image,
		paplease::HSVConfig& hsvConfig
	);

}