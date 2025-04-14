#pragma once
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

#include "base/color.h"
#include "base/documents/doc_type.h"
#include "base/ocr/font.h"
#include "base/ocr/ocr.h"
#include "base/shape.h"

namespace paplease {

	static inline cv::Mat ToGrayscale(const cv::Mat& mat)
	{
		cv::Mat grayscale;
		cv::cvtColor(mat, grayscale, cv::COLOR_BGR2GRAY);
		return grayscale;
	}

	static inline cv::Mat ApplyHSV(const cv::Mat& mat, const HSVConfig& hsvConfig)
	{
		cv::Mat imgHsv;
		cv::cvtColor(mat, imgHsv, cv::COLOR_BGR2HSV);
		cv::Mat lower{ hsvConfig.hueMin, hsvConfig.satMin, hsvConfig.valMin };
		cv::Mat upper{ hsvConfig.hueMax, hsvConfig.satMax, hsvConfig.valMax };
		cv::Mat mask;
		cv::inRange(imgHsv, lower, upper, mask);
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

	static inline cv::Mat ReadImage(const std::string& path)
	{
		auto mat = cv::imread(path, cv::IMREAD_UNCHANGED);
#if DOWNSCALE_OPTIMIZATION
		mat = ScaleImage(std::move(mat), 1.0f / 2.0f);
#endif
		return mat;
	}

	static inline cv::Mat ExtractDocumentField(const cv::Mat& document, const Rectangle& boundingBox)
	{
		return document(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height));
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
