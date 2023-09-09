#include "pch.h"
#include "base/documents/seal.h"

cv::Mat PreprocessSealedDocument(const cv::Mat& mat) {
	cv::Mat imgHsv;
	cv::cvtColor(mat, imgHsv, cv::COLOR_BGR2HSV);

	const int hueMin = 0;
	const int hueMax = 179;
	const int satMin = 0;
	const int satMax = 110;
	const int valMin = 0;
	const int valMax = 255;
	cv::Mat lower{ hueMin, satMin, valMin };
	cv::Mat upper{ hueMax, satMax, valMax };

	cv::Mat mask;
	cv::inRange(imgHsv, lower, upper, mask);

	return mask;
}

cv::Mat ExtractSeal(const cv::Mat& mat) {
	int left = -1;
	int top = -1;
	int width = -1;
	int bottom = -1;
	for (int x = 0; x < mat.cols; x++) {
		bool columnHasBlackPixel = false;
		for (int y = 0; y < mat.rows; y++) {
			if (mat.at<uchar>(y, x)) continue;
			columnHasBlackPixel = true;
			if (left == -1) left = x;
			if (top == -1 || y < top) top = y;

			if (y > bottom) bottom = y;
		}

		if (!columnHasBlackPixel && left != -1 && top != -1) {
			width = x - left;
			return mat(cv::Rect(left, top, width, bottom - top + 1));
		}
	}

	return mat(cv::Rect(left, top, width, bottom - top + 1));
}

static std::vector<cv::Mat> LoadDocumentSealsByPath(std::string&& path) {
	std::vector<cv::Mat> seals{};
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_directory()) {
			for (const auto& entry1 : std::filesystem::directory_iterator(entry.path())) {
				auto seal = cv::imread(entry1.path().generic_string(), cv::IMREAD_UNCHANGED);
				seals.push_back(seal);
			}
			continue;
		}

		auto seal = cv::imread(entry.path().generic_string(), cv::IMREAD_UNCHANGED);
		seals.push_back(seal);
	}
	return seals;
}

static std::vector<cv::Mat> LoadDocumentSeals(DocumentType documentType) {
	switch (documentType)
	{
	case DocumentType::DiplomaticAuthorization:
		return LoadDocumentSealsByPath("C:/dev/PapersPleaseAnalyzer/PapersPleaseAnalyzer/images/seals/diplomatic_authorization");
	case DocumentType::EntryPermit:
		return LoadDocumentSealsByPath("C:/dev/PapersPleaseAnalyzer/PapersPleaseAnalyzer/images/seals/entry_permit");
	case DocumentType::AccessPermit:
		return LoadDocumentSealsByPath("C:/dev/PapersPleaseAnalyzer/PapersPleaseAnalyzer/images/seals/access_permit");
	case DocumentType::WorkPass:
		return LoadDocumentSealsByPath("C:/dev/PapersPleaseAnalyzer/PapersPleaseAnalyzer/images/seals/work_pass");
	case DocumentType::GrantOfAsylum:
		return LoadDocumentSealsByPath("C:/dev/PapersPleaseAnalyzer/PapersPleaseAnalyzer/images/seals/grant_of_asylum");
	}

	return std::vector<cv::Mat>{};
}

static bool CompareSeals(const cv::Mat& seal1, const cv::Mat& seal2) {
	if (seal1.size != seal2.size) return false;
	return cv::countNonZero(seal2 - seal1) == 0;
}

bool IsValidSeal(const cv::Mat& mat, DocumentType documentType) {
	auto seals = LoadDocumentSeals(documentType);
	for (const auto& seal : seals) {
		if (CompareSeals(mat, seal)) {
			return true;
		}
	}
	return false;
}

bool IsDocumentValidlySealed(const cv::Mat& mat, DocumentType documentType) {
	auto binary = PreprocessSealedDocument(mat);
	auto seal = ExtractSeal(binary);
	return IsValidSeal(seal, documentType);
}