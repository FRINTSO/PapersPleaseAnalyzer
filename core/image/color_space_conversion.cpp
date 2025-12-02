
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

#include <paplease/colors.h>

cv::Mat to_hsv_binary(const cv::Mat &bgrMat, const hsv_config &hsvConfig)
{
	cv::Mat imgHsv;
	cv::cvtColor(bgrMat, imgHsv, cv::COLOR_BGR2HSV);
	cv::Mat lower{ hsvConfig.hueMin, hsvConfig.satMin, hsvConfig.valMin };
	cv::Mat upper{ hsvConfig.hueMax, hsvConfig.satMax, hsvConfig.valMax };
	cv::Mat mask;
	cv::inRange(imgHsv, lower, upper, mask);
	return mask;
}

cv::Mat to_hsl_binary(const cv::Mat &bgrMat, const hsl_config &hslConfig)
{
	cv::Mat imgHsl;
	cv::cvtColor(bgrMat, imgHsl, cv::COLOR_BGR2HLS);
	cv::Mat lower{ hslConfig.hueMin, hslConfig.satMin, hslConfig.litMin };
	cv::Mat upper{ hslConfig.hueMax, hslConfig.satMax, hslConfig.litMax };
	cv::Mat mask;
	cv::inRange(imgHsl, lower, upper, mask);
	return mask;
}
