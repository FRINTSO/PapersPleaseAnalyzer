#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "base/documents/document.h"
#include "base/documents/seal.h"
#include "base/layout.h"


class EntryPermit : public Document<EntryPermitLayout>, public ISealed {
public:
	using Document<EntryPermitLayout>::Document;

	bool HasValidSeal() override {
		return IsDocumentValidlySealed(mat, DocumentType::EntryPermit);
	}
};

struct EntryPermitData {
	std::string name;
	std::string passportNumber;
	std::string purpose;
	std::string duration;
	std::string expirationDate;
	bool		hasValidSeal;
};

EntryPermit FindEntryPermit(const cv::Mat& inspection);
EntryPermitData GetEntryPermitData(EntryPermit& entryPermit);

cv::Mat PreprocessEntryPermit(const EntryPermit& entryPermit);