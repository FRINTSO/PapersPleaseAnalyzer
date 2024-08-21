#include "pch.h"
#include "base/documents/grant_of_asylum.h"

#include "base/color.h"
#include "base/common.h"
#include "base/shape.h"
#include "base/image_process.h"

#include "base/documents/bounding_box_finder.inc"

namespace {
constexpr RgbColor GrantOfAsylumBorderColors[] = {
	{ 253, 222, 223 },
	{ 180, 157, 175 },
};
}

#define GRANT_OF_ASYLUM_WIDTH DOWNSCALE(320)
#define GRANT_OF_ASYLUM_HEIGHT DOWNSCALE(374)


GrantOfAsylum FindGrantOfAsylum(const cv::Mat& inspection) {
	auto boundingBox = FindDocumentBoundingBox(inspection, GrantOfAsylumBorderColors, 2);

	cv::Mat canvas(cv::Size(GRANT_OF_ASYLUM_WIDTH, GRANT_OF_ASYLUM_HEIGHT), inspection.type(), CV_RGB(255, 255, 255));

	if (boundingBox.width == GRANT_OF_ASYLUM_WIDTH && boundingBox.height == GRANT_OF_ASYLUM_HEIGHT) {
		return GrantOfAsylum{ inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height)) };
	}

	return GrantOfAsylum(cv::Mat{});
}

cv::Mat PreprocessGrantOfAsylum(const GrantOfAsylum& goa) {
	cv::Mat grayscale;
	cv::cvtColor(goa.mat, grayscale, cv::COLOR_BGR2GRAY);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, 127, 255, cv::THRESH_BINARY);

	return thresh;
}

GrantOfAsylumData GetGrantOfAsylumData(GrantOfAsylum& goa) {
	auto binary = PreprocessGrantOfAsylum(goa);

	auto name = GetFieldString(ExtractDocumentField(binary, goa.layoutProvider->GetNameBox()), DocumentType::GrantOfAsylum);
	auto country = GetFieldString(ExtractDocumentField(binary, goa.layoutProvider->GetCountryBox()), DocumentType::GrantOfAsylum);
	auto passportNumber = GetFieldString(ExtractDocumentField(binary, goa.layoutProvider->GetPassportNumberBox()), DocumentType::GrantOfAsylum);
	auto dateOfBirth = GetFieldString(ExtractDocumentField(binary, goa.layoutProvider->GetDateOfBirthBox()), DocumentType::GrantOfAsylum);
	auto height = GetFieldString(ExtractDocumentField(binary, goa.layoutProvider->GetHeightBox()), DocumentType::GrantOfAsylum);
	auto weight = GetFieldString(ExtractDocumentField(binary, goa.layoutProvider->GetWeightBox()), DocumentType::GrantOfAsylum);
	auto expirationDate = GetFieldString(ExtractDocumentField(binary, goa.layoutProvider->GetExpirationDateBox()), DocumentType::GrantOfAsylum);
	auto hasValidSeal = goa.HasValidSeal();

	return GrantOfAsylumData{ name, country, passportNumber, dateOfBirth, height, weight, expirationDate, hasValidSeal };
}