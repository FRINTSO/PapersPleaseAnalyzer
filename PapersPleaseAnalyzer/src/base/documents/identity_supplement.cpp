#include "pch.h"
#include "base/documents/identity_supplement.h"

#include "base/color.h"
#include "base/common.h"
#include "base/image_process.h"
#include "base/shape.h"

#include "base/documents/bounding_box_finder.inc"

namespace Documents::V1 {

	namespace {
		constexpr RgbColor IdentitySupplementBorderColors[] = {
			{ 232, 218, 199 },
			{ 181, 167, 166 },
		};
	}

#define IDENTITY_SUPPLEMENT_WIDTH DOWNSCALE(180)
#define IDENTITY_SUPPLEMENT_HEIGHT DOWNSCALE(300)


	IdentitySupplement FindIdentitySupplement(const cv::Mat& inspection)
	{
		auto boundingBox = FindDocumentBoundingBox(inspection, IdentitySupplementBorderColors, 2);

		cv::Mat canvas(cv::Size(IDENTITY_SUPPLEMENT_WIDTH, IDENTITY_SUPPLEMENT_HEIGHT), inspection.type(), CV_RGB(255, 255, 255));

		if (boundingBox.width == IDENTITY_SUPPLEMENT_WIDTH && boundingBox.height == IDENTITY_SUPPLEMENT_HEIGHT)
		{
			return IdentitySupplement{ inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height)) };
		}

		return IdentitySupplement(cv::Mat{});
	}

	cv::Mat PreprocessIdentitySupplement(const IdentitySupplement& identitySupplement)
	{
		cv::Mat grayscale = ToGrayscale(identitySupplement.mat);

		cv::Mat thresh;
		cv::threshold(grayscale, thresh, 127, 255, cv::THRESH_BINARY);

		return thresh;
	}

	IdentitySupplementData GetIdentitySupplementData(IdentitySupplement& idSupplement)
	{
		auto binary = PreprocessIdentitySupplement(idSupplement);

		auto height = GetFieldString(ExtractDocumentField(binary, idSupplement.layoutProvider->GetHeightBox()), DocumentType::IdentitySupplement);
		auto weight = GetFieldString(ExtractDocumentField(binary, idSupplement.layoutProvider->GetWeightBox()), DocumentType::IdentitySupplement);
		auto description = GetFieldString(ExtractDocumentField(binary, idSupplement.layoutProvider->GetDescriptionBox()), DocumentType::IdentitySupplement);
		auto expirationDate = GetFieldString(ExtractDocumentField(binary, idSupplement.layoutProvider->GetExpirationDateBox()), DocumentType::IdentitySupplement);

		return IdentitySupplementData{ height, weight, description, expirationDate };
	}

}