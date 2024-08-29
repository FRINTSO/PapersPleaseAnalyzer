#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "base/documents/document.h"
#include "base/documents/layout.h"

namespace Documents::V1 {

class Booth : public Document<BoothLayout> {
public:
	using Document<BoothLayout>::Document;

	Booth() : Document<BoothLayout>({}, nullptr) {}
};

struct BoothData {
	std::string date;
	std::string counter;
	std::string weight;
};

BoothData GetBoothData(const Booth& booth);

cv::Mat PreprocessBooth(const Booth& booth);

}