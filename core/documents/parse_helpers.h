#ifndef PAPLEASE_PARSE_HELPERS_H
#define PAPLEASE_PARSE_HELPERS_H

#include "paplease/resources.h"
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
#include <paplease/documents.h>
#include <paplease/geometry.h>
#include <string>
#include <cstdio>

inline cv::Mat preprocess_document(const cv::Mat &img, int threshValue = 127,
				   int thresholdType = cv::THRESH_BINARY)
{
	cv::Mat grayscale;
	if (img.channels() == 1)
		grayscale = img;
	else
		cv::cvtColor(img, grayscale, cv::COLOR_BGR2GRAY);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, threshValue, 255, thresholdType);
	return thresh;
}

// Safely extract a region, returns empty Mat if out of bounds
inline cv::Mat safe_region(const cv::Mat &img, const rectangle &box)
{
	if (box.x < 0 || box.y < 0)
		return {};
	if (box.x + box.width > img.cols || box.y + box.height > img.rows)
		return {};
	return img(box.to_cv());
}

// Extract text from a region, returns false if region invalid or OCR fails
inline bool extract_field(std::string &out, const cv::Mat &img,
			  const rectangle &box, typeface tf,
			  const resources_ctx &ctx)
{
	cv::Mat region = safe_region(img, box);
	if (region.empty())
		return false;
	return extract_text_strict(out, region, tf, ctx);
}

inline bool parse_height(u16 &out, const std::string &text)
{
	int val;
	if (sscanf(text.c_str(), "%dcm", &val) != 1)
		return false;
	out = (u16)val;
	return true;
}

inline bool parse_weight(u16 &out, const std::string &text)
{
	int val;
	if (sscanf(text.c_str(), "%dkg", &val) != 1)
		return false;
	out = (u16)val;
	return true;
}
#endif // PAPLEASE_PARSE_HELPERS_H
