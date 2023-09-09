#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "base/color.h"
#include "base/common.h"
#include "base/documents/document.h"
#include "base/layout.h"


namespace {
	constexpr RgbColor EntryTicketBorderColors[] = {
		{ 224, 233, 199 },
		{ 180, 169, 151 },
	};
}

#define ENTRY_TICKET_WIDTH DOWNSCALE(280)
#define ENTRY_TICKET_HEIGHT DOWNSCALE(102)

class EntryTicket : public Document<EntryTicketLayout> {
public:
	using Document<EntryTicketLayout>::Document;
};

struct EntryTicketData {
	std::string validDate;
};

EntryTicket FindEntryTicket(const cv::Mat& inspection);
EntryTicketData GetEntryTicketData(EntryTicket& entryTicket);

cv::Mat PreprocessEntryTicket(const EntryTicket& entryTicket);