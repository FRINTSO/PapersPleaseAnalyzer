#pragma once
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

#include "paplease/common/color.h"
#include "paplease/common/shape.h"
#include "paplease/documents/doc_type.h"
#include "paplease/ocr/font.h"
#include "paplease/ocr/ocr.h"

namespace paplease {

	static inline cv::Mat ToGrayscale(const cv::Mat& bgrMat)
	{
		cv::Mat grayscale;
		cv::cvtColor(bgrMat, grayscale, cv::COLOR_BGR2GRAY);
		return grayscale;
	}

	static inline cv::Mat ToHSVBinary(const cv::Mat& bgrMat, const HSVConfig& hsvConfig)
	{
		cv::Mat imgHsv;
		cv::cvtColor(bgrMat, imgHsv, cv::COLOR_BGR2HSV);
		cv::Mat lower{ hsvConfig.hueMin, hsvConfig.satMin, hsvConfig.valMin };
		cv::Mat upper{ hsvConfig.hueMax, hsvConfig.satMax, hsvConfig.valMax };
		cv::Mat mask;
		cv::inRange(imgHsv, lower, upper, mask);
		return mask;
	}

	static inline cv::Mat ToHSLBinary(const cv::Mat& bgrMat, const HSLConfig& hslConfig)
	{
		cv::Mat imgHsl;
		cv::cvtColor(bgrMat, imgHsl, cv::COLOR_BGR2HLS);
		cv::Mat lower{ hslConfig.hueMin, hslConfig.satMin, hslConfig.litMin };
		cv::Mat upper{ hslConfig.hueMax, hslConfig.satMax, hslConfig.litMax };
		cv::Mat mask;
		cv::inRange(imgHsl, lower, upper, mask);
		return mask;
	}

	static inline cv::Mat ScaleImage(cv::Mat&& in, float scale)
	{
		cv::resize(in, in, cv::Size((int)((float)in.cols * scale), (int)((float)in.rows * scale)), 0, 0, cv::INTER_NEAREST);
		return in;
	}

	static inline cv::Mat ScaleImage(const cv::Mat& in, float scale)
	{
		cv::Mat result;
		cv::resize(in, result, cv::Size((int)((float)in.cols * scale), (int)((float)in.rows * scale)), 0, 0, cv::INTER_NEAREST);
		return result;
	}

	static inline cv::Mat LoadImageFile(const std::string& path)
	{
		auto mat = cv::imread(path, cv::IMREAD_UNCHANGED);
#if DOWNSCALE_OPTIMIZATION
		mat = ScaleImage(std::move(mat), 1.0f / 2.0f);
#endif
		return mat;
	}

	static inline cv::Mat ExtractDocumentField(const cv::Mat& document, const Rectangle& boundingBox)
	{
#define DEBUG_SHOW_BOX 0
#if DEBUG_SHOW_BOX
		cv::Mat temp;
		document.copyTo(temp);
		cv::rectangle(temp, boundingBox, cv::Scalar(127));
		cv::imshow("Box", temp);
		cv::waitKey();
#endif
		return document(boundingBox);
	}

	static inline std::string GetFieldString(const cv::Mat& field, documents::DocType documentType)
	{
		const ocr::FontInfo& fontInfo = ocr::GetFontInfo(documentType);
		return ocr::ImageToString(field, fontInfo);
	}

	static inline std::string GetBoothString(const cv::Mat& field)
	{
		const ocr::FontInfo& fontInfo = ocr::GetBoothFontInfo();
		return ocr::ImageToString(field, fontInfo);
	}

}  // namespace paplease
