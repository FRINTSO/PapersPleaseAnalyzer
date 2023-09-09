#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "base/color.h"
#include "base/common.h"
#include "base/documents/document.h"
#include "base/layout.h"


namespace {
	constexpr RgbColor IdentitySupplementBorderColors[] = {
		{ 232, 218, 199 },
		{ 181, 167, 166 },
	};
}

#define IDENTITY_SUPPLEMENT_WIDTH DOWNSCALE(180)
#define IDENTITY_SUPPLEMENT_HEIGHT DOWNSCALE(300)

class IdentitySupplement : public Document<IdentitySupplementLayout> {
public:
	using Document<IdentitySupplementLayout>::Document;
};

struct IdentitySupplementData {
	std::string height;
	std::string weight;
	std::string description;
	std::string expirationDate;
};

IdentitySupplement FindIdentitySupplement(const cv::Mat& inspection);
IdentitySupplementData GetIdentitySupplementData(IdentitySupplement& idSupplement);

cv::Mat PreprocessIdentitySupplement(const IdentitySupplement& identitySupplement);