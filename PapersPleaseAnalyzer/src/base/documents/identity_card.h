#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "base/color.h"
#include "base/common.h"
#include "base/documents/document.h"
#include "base/layout.h"


namespace {
	constexpr RgbColor IdentitiyCardBorderColors[] = {
		{ 217, 189, 247 },
		{ 178, 156, 204 },
	};
}

#define IDENTITY_CARD_WIDTH DOWNSCALE(252)
#define IDENTITY_CARD_HEIGHT DOWNSCALE(142)

class IdentityCard : public Document<IdentityCardLayout> {
public:
	using Document<IdentityCardLayout>::Document;
};

struct IdentityCardData {
	std::string district;
	std::string name;
	std::string dateOfBirth;
	std::string height;
	std::string weight;
};

IdentityCard FindIdentityCard(const cv::Mat& inspection);
IdentityCardData GetIdentityCardData(const IdentityCard& identityCard);

cv::Mat PreprocessIdentityCard(const IdentityCard& identityCard);