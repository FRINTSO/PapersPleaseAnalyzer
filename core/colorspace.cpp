#include <opencv2/imgproc.hpp>

#include <paplease/colorspace.h>

cv::Mat bgr_to_hsv_mask(const cv::Mat &bgr_image, const hsv_range &range)
{
	cv::Mat image_hsv;
	cv::cvtColor(bgr_image, image_hsv, cv::COLOR_BGR2HSV);
	cv::Mat lower_bound{ range.hue_min, range.sat_min, range.val_min };
	cv::Mat upper_bound{ range.hue_max, range.sat_max, range.val_max };
	cv::Mat mask;
	cv::inRange(image_hsv, lower_bound, upper_bound, mask);
	return mask;
}

cv::Mat bgr_to_hsl_mask(const cv::Mat &bgr_image, const hsl_range &range)
{
	cv::Mat image_gsl;
	cv::cvtColor(bgr_image, image_gsl, cv::COLOR_BGR2HLS);
	cv::Mat lower_bound{ range.hue_min, range.sat_min, range.lit_min };
	cv::Mat upper_bound{ range.hue_max, range.sat_max, range.lit_max };
	cv::Mat mask;
	cv::inRange(image_gsl, lower_bound, upper_bound, mask);
	return mask;
}
