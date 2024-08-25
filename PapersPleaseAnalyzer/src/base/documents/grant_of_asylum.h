#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "base/documents/document.h"
#include "base/documents/seal.h"
#include "base/documents/layout.h"

namespace Documents::V1 {

	class GrantOfAsylum : public Document<GrantOfAsylumLayout>, public ISealed
	{
	public:
		using Document<GrantOfAsylumLayout>::Document;

		bool HasValidSeal() override
		{
			return IsDocumentValidlySealed(mat, DocumentType::GrantOfAsylum);
		}
	};

	struct GrantOfAsylumData
	{
		std::string name;
		std::string country;
		std::string passportNumber;
		std::string dateOfBirth;
		std::string height;
		std::string weight;
		std::string expirationDate;
		bool		hasValidSeal;
	};

	GrantOfAsylum FindGrantOfAsylum(const cv::Mat& inspection);
	GrantOfAsylumData GetGrantOfAsylumData(GrantOfAsylum& goa);

	cv::Mat PreprocessGrantOfAsylum(const GrantOfAsylum& goa);

}