#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "base/color.h"
#include "base/common.h"
#include "base/documents/document.h"
#include "base/documents/seal.h"
#include "base/layout.h"


namespace {
	constexpr RgbColor DiplomaticAuthorizationBorderColors[] = {
		{ 238, 238, 251 },
		{ 190, 202, 209 },
		{ 154, 179, 168 },
	};
}

#define DIPLOMATIC_AUTHORIZATION_WIDTH DOWNSCALE(300)
#define DIPLOMATIC_AUTHORIZATION_HEIGHT DOWNSCALE(400)

class DiplomaticAuthorization : public Document<DiplomaticAuthorizationLayout>, public ISealed {
public:
	using Document<DiplomaticAuthorizationLayout>::Document;

	bool HasValidSeal() override {
		return IsDocumentValidlySealed(mat, DocumentType::DiplomaticAuthorization);
	}
};

struct DiplomaticAuthorizationData {
	std::string name;
	std::string passportNumber;
	std::string issuingCountry;
	std::string countryList;
	bool		hasValidSeal;
};

DiplomaticAuthorization FindDiplomaticAuthorization(const cv::Mat& inspection);
DiplomaticAuthorizationData GetDiplomaticAuthorizationData(DiplomaticAuthorization& auth);

cv::Mat PreprocessDiplomaticAuthorization(const DiplomaticAuthorization& diplomaticAuthorization);