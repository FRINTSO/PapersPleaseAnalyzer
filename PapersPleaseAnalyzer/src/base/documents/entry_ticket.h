#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "base/documents/document.h"
#include "base/documents/layout.h"

namespace Documents::V1 {

	class EntryTicket : public Document<EntryTicketLayout>
	{
	public:
		using Document<EntryTicketLayout>::Document;
	};

	struct EntryTicketData
	{
		std::string validDate;
	};

	EntryTicket FindEntryTicket(const cv::Mat& inspection);
	EntryTicketData GetEntryTicketData(EntryTicket& entryTicket);

	cv::Mat PreprocessEntryTicket(const EntryTicket& entryTicket);

}