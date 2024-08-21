#include "pch.h"
#include "base/documents/diplomatic_authorization.h"

#include "base/color.h"
#include "base/common.h"
#include "base/image_process.h"
#include "base/shape.h"

#include "base/documents/bounding_box_finder.inc"

namespace {
constexpr RgbColor DiplomaticAuthorizationBorderColors[] = {
	{ 238, 238, 251 },
	{ 190, 202, 209 },
	{ 154, 179, 168 },
};
}

#define DIPLOMATIC_AUTHORIZATION_WIDTH DOWNSCALE(300)
#define DIPLOMATIC_AUTHORIZATION_HEIGHT DOWNSCALE(400)


DiplomaticAuthorization FindDiplomaticAuthorization(const cv::Mat& inspection) {
	auto boundingBox = FindDocumentBoundingBox(inspection, DiplomaticAuthorizationBorderColors, 3);

	cv::Mat canvas(cv::Size(DIPLOMATIC_AUTHORIZATION_WIDTH, DIPLOMATIC_AUTHORIZATION_HEIGHT), inspection.type(), CV_RGB(255, 255, 255));

	if (boundingBox.width == DIPLOMATIC_AUTHORIZATION_WIDTH && boundingBox.height == DIPLOMATIC_AUTHORIZATION_HEIGHT) {
		return DiplomaticAuthorization{ inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height)) };
	}

	return DiplomaticAuthorization(cv::Mat{});
}

cv::Mat PreprocessDiplomaticAuthorization(const DiplomaticAuthorization& diplomaticAuthorization) {
	cv::Mat grayscale = ToGrayscale(diplomaticAuthorization.mat);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, 128, 255, cv::THRESH_BINARY);
	return thresh;
}

DiplomaticAuthorizationData GetDiplomaticAuthorizationData(DiplomaticAuthorization& auth) {
	auto binaryAuth = PreprocessDiplomaticAuthorization(auth);

	auto name = GetFieldString(ExtractDocumentField(binaryAuth, auth.layoutProvider->GetNameBox()), DocumentType::DiplomaticAuthorization);
	auto passportNumber = GetFieldString(ExtractDocumentField(binaryAuth, auth.layoutProvider->GetPassportNumberBox()), DocumentType::DiplomaticAuthorization);
	auto issuingCountry = GetFieldString(ExtractDocumentField(binaryAuth, auth.layoutProvider->GetIssuingCountryBox()), DocumentType::DiplomaticAuthorization);
	auto countryList = GetFieldString(ExtractDocumentField(binaryAuth, auth.layoutProvider->GetCountryListBox()), DocumentType::DiplomaticAuthorization);
	auto hasValidSeal = auth.HasValidSeal();

	return DiplomaticAuthorizationData{ name, passportNumber, issuingCountry, countryList, hasValidSeal };
}
