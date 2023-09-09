#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "base/documents/document.h"
#include "base/ocr/font.h"
#include "base/ocr/ocr.h"

inline cv::Mat ToGrayscale(const cv::Mat& mat) {
	cv::Mat grayscale;
	cv::cvtColor(mat, grayscale, cv::COLOR_BGR2GRAY);
	return grayscale;
}

inline std::string GetFieldString(const cv::Mat& field, DocumentType documentType) {
	const FontInfo fontInfo = GetFontInfo(documentType);
	return ImageToString(field, fontInfo);
}