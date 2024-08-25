#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "base/documents/document.h"
#include "base/documents/seal.h"
#include "base/documents/layout.h"

namespace Documents::V1 {

	class DiplomaticAuthorization : public Document<DiplomaticAuthorizationLayout>, public ISealed
	{
	public:
		using Document<DiplomaticAuthorizationLayout>::Document;

		bool HasValidSeal() override
		{
			return IsDocumentValidlySealed(mat, DocumentType::DiplomaticAuthorization);
		}
	};

	struct DiplomaticAuthorizationData
	{
		std::string name;
		std::string passportNumber;
		std::string issuingCountry;
		std::string countryList;
		bool		hasValidSeal;
	};

	DiplomaticAuthorization FindDiplomaticAuthorization(const cv::Mat& inspection);
	DiplomaticAuthorizationData GetDiplomaticAuthorizationData(DiplomaticAuthorization& auth);

	cv::Mat PreprocessDiplomaticAuthorization(const DiplomaticAuthorization& diplomaticAuthorization);

}