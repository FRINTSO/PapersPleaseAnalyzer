#include "pch.h"
#include "base/booth.h"

#include "base/common.h"
#include "base/image_process.h"

namespace Documents::V1 {

	cv::Mat PreprocessBooth(const Booth& booth)
	{
		cv::Mat imgHsv;
		cv::cvtColor(booth.mat, imgHsv, cv::COLOR_BGR2HSV);

		const int hueMin = 12;
		const int hueMax = 18;
		const int satMin = 0;
		const int satMax = 255;
		const int valMin = 106;
		const int valMax = 106;
		cv::Mat lower{ hueMin, satMin, valMin };
		cv::Mat upper{ hueMax, satMax, valMax };

		cv::Mat mask;
		cv::inRange(imgHsv, lower, upper, mask);

		cv::Mat insetImage(mask, cv::Rect(DOWNSCALE(168), DOWNSCALE(416), DOWNSCALE(22), DOWNSCALE(12)));
		cv::Mat alter = (insetImage ^ 255);
		alter.copyTo(insetImage);

		return mask;
	}

	//cv::Mat PreprocessBooth(const Booth& booth) {
	//	cv::Mat grayscale = ToGrayscale(booth.mat);
	//
	//	cv::Mat thresh;
	//	cv::threshold(grayscale, thresh, 48, 255, cv::THRESH_BINARY);
	//
	//	cv::Mat insetImage(thresh, cv::Rect(DOWNSCALE(168), DOWNSCALE(416), DOWNSCALE(22), DOWNSCALE(10)));
	//	cv::Mat alter = (insetImage ^ 255);
	//	alter.copyTo(insetImage);
	//
	//	cv::imshow("Grayscale", grayscale);
	//
	//	return thresh;
	//}

	BoothData GetBoothData(const Booth& booth)
	{
		auto binaryBooth = PreprocessBooth(booth.mat);

		auto date = GetFieldString(ExtractDocumentField(binaryBooth, booth.layoutProvider->GetDateBox()), DocumentType::Booth);
		auto counter = GetFieldString(ExtractDocumentField(binaryBooth, booth.layoutProvider->GetCounterBox()), DocumentType::Booth);
		auto weight = GetFieldString(ExtractDocumentField(binaryBooth, booth.layoutProvider->GetWeightBox()), DocumentType::Booth);

		return BoothData{ date, counter, weight };
	}

}