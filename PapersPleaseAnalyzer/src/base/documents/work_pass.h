#pragma once
#include <array>
#include <string>

#include <opencv2/opencv.hpp>

#include "base/documents/document.h"
#include "base/documents/seal.h"
#include "base/layout.h"


class WorkPass : public Document<WorkPassLayout>, public ISealed {
public:
	using Document<WorkPassLayout>::Document;

	bool HasValidSeal() override {
		return IsDocumentValidlySealed(mat, DocumentType::WorkPass);
	}
};

struct WorkPassData {
	std::string name;
	std::string field;
	std::string endDate;
	bool		hasValidSeal;
};

WorkPass FindWorkPass(const cv::Mat& inspection);
WorkPassData GetWorkPassData(WorkPass& workPass);

cv::Mat PreprocessWorkPass(const WorkPass& workPass);