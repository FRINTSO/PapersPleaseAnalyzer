#pragma once
#include <string>

#include "base/color.h"
#include "base/common.h"
#include "base/documents/document.h"
#include "base/documents/seal.h"
#include "base/layout.h"

namespace {
	constexpr RgbColor AccessPermitBorderColors[] = {
		{ 215, 233, 210 },
		{ 176, 168, 173 },
	};
}

#define ACCESS_PERMIT_WIDTH DOWNSCALE(294)
#define ACCESS_PERMIT_HEIGHT DOWNSCALE(392)

class AccessPermit : public Document<AccessPermitLayout>, public ISealed {
public:
	using Document<AccessPermitLayout>::Document;

	bool HasValidSeal() override {
		return IsDocumentValidlySealed(mat, DocumentType::AccessPermit);
	}
};

struct AccessPermitData {
	std::string name;
	std::string country;
	std::string passportNumber;
	std::string purpose;
	std::string durationOfStay;
	std::string height;
	std::string weight;
	std::string physicalAppearance;
	std::string expirationDate;
	bool		hasValidSeal;
};

AccessPermit FindAccessPermit(const cv::Mat& inspection);
AccessPermitData GetAccessPermitData(AccessPermit& accessPermit);

cv::Mat PreprocessAccessPermit(const AccessPermit& accessPermit);