#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "base/documents/document.h"
#include "base/documents/layout.h"

namespace Documents::V1 {

	class IdentitySupplement : public Document<IdentitySupplementLayout>
	{
	public:
		using Document<IdentitySupplementLayout>::Document;
	};

	struct IdentitySupplementData
	{
		std::string height;
		std::string weight;
		std::string description;
		std::string expirationDate;
	};

	IdentitySupplement FindIdentitySupplement(const cv::Mat& inspection);
	IdentitySupplementData GetIdentitySupplementData(IdentitySupplement& idSupplement);

	cv::Mat PreprocessIdentitySupplement(const IdentitySupplement& identitySupplement);

}