#include "pch.h"
#include "base/documents/entry_permit.h"

#include "base/image_process.h"
#include "base/shape.h"

#include "base/documents/bounding_box_finder.inc"

EntryPermit FindEntryPermit(const cv::Mat& inspection) {
	auto boundingBox = FindDocumentBoundingBox(inspection, EntryPermitBorderColors, 2);
	
	cv::Mat canvas(cv::Size(ENTRY_PERMIT_WIDTH, ENTRY_PERMIT_HEIGHT), inspection.type(), CV_RGB(255, 255, 255));
	//cv::Mat canvas = cv::Mat::zeros(cv::Size(ENTRY_PERMIT_WIDTH, ENTRY_PERMIT_HEIGHT), inspection.type());

	if (boundingBox.width == ENTRY_PERMIT_WIDTH && boundingBox.height == ENTRY_PERMIT_HEIGHT) {
		return EntryPermit{ inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height)) };
	}

	return EntryPermit(cv::Mat{});
}

cv::Mat PreprocessEntryPermit(const EntryPermit& entryPermit) {
	cv::Mat grayscale = ToGrayscale(entryPermit.mat);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, 127, 255, cv::THRESH_BINARY);

	return thresh;
}

EntryPermitData GetEntryPermitData(EntryPermit& entryPermit) {
	auto binaryEntryPermit = PreprocessEntryPermit(entryPermit);

	auto name = GetFieldString(ExtractDocumentField(binaryEntryPermit, entryPermit.layoutProvider->GetNameBox()), DocumentType::EntryPermit);
	auto passportNumber = GetFieldString(ExtractDocumentField(binaryEntryPermit, entryPermit.layoutProvider->GetPassportNumberBox()), DocumentType::EntryPermit);
	auto purpose = GetFieldString(ExtractDocumentField(binaryEntryPermit, entryPermit.layoutProvider->GetPurposeBox()), DocumentType::EntryPermit);
	auto duration = GetFieldString(ExtractDocumentField(binaryEntryPermit, entryPermit.layoutProvider->GetDurationOfStayBox()), DocumentType::EntryPermit);
	auto expirationDate = GetFieldString(ExtractDocumentField(binaryEntryPermit, entryPermit.layoutProvider->GetExpirationDateBox()), DocumentType::EntryPermit);
	auto hasValidSeal = entryPermit.HasValidSeal();

	return EntryPermitData{ name, passportNumber, purpose, duration, expirationDate, hasValidSeal };
}