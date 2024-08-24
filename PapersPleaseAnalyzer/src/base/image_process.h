#pragma once
#include <string>
#include <vector>

#include <opencv2/opencv.hpp>

#include "base/documents/document.h"
#include "base/documents_v2/doc_type.h"
#include "base/ocr/font.h"
#include "base/ocr/ocr.h"
#include "base/shape.h"

inline cv::Mat ToGrayscale(const cv::Mat& mat) {
	cv::Mat grayscale;
	cv::cvtColor(mat, grayscale, cv::COLOR_BGR2GRAY);
	return grayscale;
}

inline std::string GetFieldString(const cv::Mat& field, DocumentType documentType) {
	const FontInfo fontInfo = GetFontInfo(documentType);
	return ImageToString(field, fontInfo);
}

inline cv::Mat ExtractDocumentField(const cv::Mat& document, const Rectangle& boundingBox) {
	return document(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height));
}

inline std::string GetFieldString(const cv::Mat& field, Documents::V2::DocType documentType) {
	const FontInfo fontInfo = GetFontInfo(documentType);
	return ImageToString(field, fontInfo);
}