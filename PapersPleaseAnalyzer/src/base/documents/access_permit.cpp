#include "pch.h"
#include "base/documents/access_permit.h"

#include "base/image_process.h"
#include "base/shape.h"

#include "base/documents/bounding_box_finder.inc"

AccessPermit FindAccessPermit(const cv::Mat& inspection) {
	auto boundingBox = FindDocumentBoundingBox(inspection, AccessPermitBorderColors, 2);

	cv::Mat canvas(cv::Size(ACCESS_PERMIT_WIDTH, ACCESS_PERMIT_HEIGHT), inspection.type(), CV_RGB(255, 255, 255));

	if (boundingBox.width == ACCESS_PERMIT_WIDTH && boundingBox.height == ACCESS_PERMIT_HEIGHT) {
		return AccessPermit{ inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height)) };
	}

	return AccessPermit(cv::Mat{});
}

cv::Mat PreprocessAccessPermit(const AccessPermit& accessPermit) {
	cv::Mat grayscale = ToGrayscale(accessPermit.mat);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, 127, 255, cv::THRESH_BINARY);

	return thresh;
}

AccessPermitData GetAccessPermitData(AccessPermit& accessPermit) {
	auto binary = PreprocessAccessPermit(accessPermit);

	auto name = GetFieldString(ExtractDocumentField(binary, accessPermit.layoutProvider->GetNameBox()), DocumentType::AccessPermit);
	auto country = GetFieldString(ExtractDocumentField(binary, accessPermit.layoutProvider->GetCountryBox()), DocumentType::AccessPermit);
	auto passportNumber = GetFieldString(ExtractDocumentField(binary, accessPermit.layoutProvider->GetPassportNumberBox()), DocumentType::AccessPermit);
	auto purpose = GetFieldString(ExtractDocumentField(binary, accessPermit.layoutProvider->GetPurposeBox()), DocumentType::AccessPermit);
	auto durationOfStay = GetFieldString(ExtractDocumentField(binary, accessPermit.layoutProvider->GetDurationOfStayBox()), DocumentType::AccessPermit);
	auto height = GetFieldString(ExtractDocumentField(binary, accessPermit.layoutProvider->GetHeightBox()), DocumentType::AccessPermit);
	auto weight = GetFieldString(ExtractDocumentField(binary, accessPermit.layoutProvider->GetWeightBox()), DocumentType::AccessPermit);
	auto physicalAppearance = GetFieldString(ExtractDocumentField(binary, accessPermit.layoutProvider->GetPhysicalAppearanceBox()), DocumentType::AccessPermit);
	auto expirationDate = GetFieldString(ExtractDocumentField(binary, accessPermit.layoutProvider->GetExpirationDateBox()), DocumentType::AccessPermit);
	auto hasValidSeal = accessPermit.HasValidSeal();

	return AccessPermitData{ name, country, passportNumber, purpose, durationOfStay, height, weight, physicalAppearance, expirationDate, hasValidSeal };
}