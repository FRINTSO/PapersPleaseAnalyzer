#pragma once
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

#include "base/documents/document.h"
#include "base/documents_v2/doc_type.h"
#include "base/ocr/font.h"
#include "base/ocr/ocr.h"
#include "base/shape.h"

static inline cv::Mat ToGrayscale(const cv::Mat& mat) {
  	cv::Mat grayscale;
	cv::cvtColor(mat, grayscale, cv::COLOR_BGR2GRAY);
	return grayscale;
}


static inline cv::Mat ScaleImage(const cv::Mat& in, float scale)
{
	cv::Mat result;
	cv::resize(in, result, cv::Size((int)((float)in.cols / (1.0f / scale)), (int)((float)in.rows / (1.0f / scale))), 0, 0, cv::INTER_NEAREST);
	return result;
}

static inline std::string GetFieldString(const cv::Mat& field, Documents::V1::DocumentType documentType) {
	const FontInfo& fontInfo = GetFontInfo(documentType);
	return ImageToString(field, fontInfo);
}

static inline cv::Mat ExtractDocumentField(const cv::Mat& document, const Rectangle& boundingBox) {
	return document(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height));
}

static inline std::string GetFieldString(const cv::Mat& field, Documents::V2::DocType documentType) {
	const FontInfo& fontInfo = GetFontInfo(documentType);
	return ImageToString(field, fontInfo);
}