#include "pch.h"
#include "base/documents/work_pass.h"

#include <array>

#include "base/shape.h"
#include "base/image_process.h"

#include "base/documents/bounding_box_finder.inc"

WorkPass FindWorkPass(const cv::Mat& inspection) {
	auto boundingBox = FindDocumentBoundingBox(inspection, WorkPassBorderColors, 2);
	
	cv::Mat canvas(cv::Size(WORK_PASS_WIDTH, WORK_PASS_HEIGHT), inspection.type(), CV_RGB(255, 255, 255));
	//cv::Mat canvas = cv::Mat::zeros(cv::Size(WORK_PASS_WIDTH, WORK_PASS_HEIGHT), inspection.type());

	if (boundingBox.width == WORK_PASS_WIDTH && boundingBox.height == WORK_PASS_HEIGHT) {
		return WorkPass{ inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height)) };
	}

	return WorkPass(cv::Mat{});
}

cv::Mat PreprocessWorkPass(const WorkPass& workPass) {
	cv::Mat grayscale = ToGrayscale(workPass.mat);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, 127, 255, cv::THRESH_BINARY);

	return thresh;
}

WorkPassData GetWorkPassData(WorkPass& workPass) {
	auto binaryWorkPass = PreprocessWorkPass(workPass);

	auto name = GetFieldString(ExtractDocumentField(binaryWorkPass, workPass.layoutProvider->GetNameBox()), DocumentType::WorkPass);
	auto field = GetFieldString(ExtractDocumentField(binaryWorkPass, workPass.layoutProvider->GetFieldBox()), DocumentType::WorkPass);
	auto endDate = GetFieldString(ExtractDocumentField(binaryWorkPass, workPass.layoutProvider->GetEndDateBox()), DocumentType::WorkPass);
	auto hasValidSeal = workPass.HasValidSeal();

	return WorkPassData{ name, field, endDate, hasValidSeal };
}