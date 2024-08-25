#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "base/documents/document.h"
#include "base/documents/layout.h"

namespace Documents::V1 {

	class IdentityCard : public Document<IdentityCardLayout>
	{
	public:
		using Document<IdentityCardLayout>::Document;
	};

	struct IdentityCardData
	{
		std::string district;
		std::string name;
		std::string dateOfBirth;
		std::string height;
		std::string weight;
	};

	IdentityCard FindIdentityCard(const cv::Mat& inspection);
	IdentityCardData GetIdentityCardData(const IdentityCard& identityCard);

	cv::Mat PreprocessIdentityCard(const IdentityCard& identityCard);

}