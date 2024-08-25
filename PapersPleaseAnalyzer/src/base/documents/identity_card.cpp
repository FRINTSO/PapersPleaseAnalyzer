#include "pch.h"
#include "base/documents/identity_card.h"

#include "base/color.h"
#include "base/common.h"
#include "base/image_process.h"
#include "base/shape.h"

#include "base/documents/bounding_box_finder.inc"

namespace Documents::V1 {

	namespace {
		constexpr RgbColor IdentitiyCardBorderColors[] = {
			{ 217, 189, 247 },
			{ 178, 156, 204 },
		};
	}

#define IDENTITY_CARD_WIDTH DOWNSCALE(252)
#define IDENTITY_CARD_HEIGHT DOWNSCALE(142)


	IdentityCard FindIdentityCard(const cv::Mat& inspection)
	{
		auto boundingBox = FindDocumentBoundingBox(inspection, IdentitiyCardBorderColors, 2);

		cv::Mat canvas(cv::Size(IDENTITY_CARD_WIDTH, IDENTITY_CARD_HEIGHT), inspection.type(), CV_RGB(255, 255, 255));
		//cv::Mat canvas = cv::Mat::zeros(cv::Size(IDENTITY_CARD_WIDTH, IDENTITY_CARD_HEIGHT), inspection.type());

		if (boundingBox.width == IDENTITY_CARD_WIDTH && boundingBox.height == IDENTITY_CARD_HEIGHT)
		{
			return IdentityCard{ inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height)) };
		}


		return IdentityCard(canvas);
	}

	cv::Mat PreprocessIdentityCard(const IdentityCard& identityCard)
	{
		cv::Mat grayscale = ToGrayscale(identityCard.mat);

		cv::Mat thresh;
		cv::threshold(grayscale, thresh, 127, 255, cv::THRESH_BINARY);

		cv::Mat insetImage(thresh, cv::Rect(DOWNSCALE(6), DOWNSCALE(20), DOWNSCALE(238), DOWNSCALE(14)));
		cv::Mat alter = (insetImage ^ 255);
		alter.copyTo(insetImage);

		return thresh;
	}

	IdentityCardData GetIdentityCardData(const IdentityCard& identityCard)
	{
		auto binaryIdentityCard = PreprocessIdentityCard(identityCard);

		auto district = GetFieldString(ExtractDocumentField(binaryIdentityCard, identityCard.layoutProvider->GetDistrictBox()), DocumentType::IdentityCard);
		auto name = GetFieldString(ExtractDocumentField(binaryIdentityCard, identityCard.layoutProvider->GetNameBox()), DocumentType::IdentityCard);
		auto dateOfBirth = GetFieldString(ExtractDocumentField(binaryIdentityCard, identityCard.layoutProvider->GetDateOfBirthBox()), DocumentType::IdentityCard);
		auto height = GetFieldString(ExtractDocumentField(binaryIdentityCard, identityCard.layoutProvider->GetHeightBox()), DocumentType::IdentityCard);
		auto weight = GetFieldString(ExtractDocumentField(binaryIdentityCard, identityCard.layoutProvider->GetWeightBox()), DocumentType::IdentityCard);

		return IdentityCardData{ district, name, dateOfBirth, height, weight };
	}

}