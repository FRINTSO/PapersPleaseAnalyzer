#include "pch.h"
#include "base/documents/entry_ticket.h"

#include "base/image_process.h"
#include "base/shape.h"

#include "base/documents/bounding_box_finder.inc"

EntryTicket FindEntryTicket(const cv::Mat& inspection) {
	auto boundingBox = FindDocumentBoundingBox(inspection, EntryTicketBorderColors, 2);

	cv::Mat canvas(cv::Size(ENTRY_TICKET_WIDTH, ENTRY_TICKET_HEIGHT), inspection.type(), CV_RGB(255, 255, 255));

	if (boundingBox.width == ENTRY_TICKET_WIDTH && boundingBox.height == ENTRY_TICKET_HEIGHT) {
		return EntryTicket{ inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height)) };
	}

	return EntryTicket(cv::Mat{});
}

cv::Mat PreprocessEntryTicket(const EntryTicket& entryTicket) {
	cv::Mat grayscale = ToGrayscale(entryTicket.mat);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, 127, 255, cv::THRESH_BINARY);

	return thresh;
}

EntryTicketData GetEntryTicketData(EntryTicket& entryTicket) {
	auto binary = PreprocessEntryTicket(entryTicket);

	auto validDate = GetFieldString(ExtractDocumentField(binary, entryTicket.layoutProvider->GetValidDateBox()), DocumentType::EntryTicket);

	return EntryTicketData{ validDate };
}