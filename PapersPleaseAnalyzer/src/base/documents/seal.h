#pragma once
#include<opencv2/opencv.hpp>

#include "base/documents/document.h"

namespace Documents::V1
{

	class ISealed
	{
	public:
		virtual ~ISealed() {}
		virtual bool HasValidSeal() = 0;
	};

	cv::Mat PreprocessSealedDocument(const cv::Mat& mat);
	cv::Mat ExtractSeal(const cv::Mat& mat);
	bool IsValidSeal(const cv::Mat& mat, DocumentType documentType);
	bool IsDocumentValidlySealed(const cv::Mat& mat, DocumentType documentType);

}

