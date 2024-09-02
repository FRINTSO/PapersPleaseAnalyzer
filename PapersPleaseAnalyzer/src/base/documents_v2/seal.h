#pragma once
#include<opencv2/opencv.hpp>

#include "base/documents_v2/doc_class.h"

namespace Documents::V2
{

	//cv::Mat PreprocessSealedDocument(const cv::Mat& mat);
	//cv::Mat ExtractSeal(const cv::Mat& mat);
	//bool IsValidSeal(const cv::Mat& mat, DocType documentType);
	bool IsDocumentValidlySealed(const cv::Mat& mat, DocType documentType);

}