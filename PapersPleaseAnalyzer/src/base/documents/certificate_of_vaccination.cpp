#include "pch.h"
#include "base/documents/certificate_of_vaccination.h"

#include "base/color.h"
#include "base/common.h"
#include "base/image_process.h"
#include "base/shape.h"

#include "base/documents/bounding_box_finder.inc"

namespace Documents::V1 {

	namespace {
		constexpr RgbColor CertificateOfVaccinationBorderColors[] = {
			{ 234, 223, 128 },
			{ 177, 151, 97 },
		};
	}

#define CERTIFICATE_OF_VACCINATION_WIDTH DOWNSCALE(270)
#define CERTIFICATE_OF_VACCINATION_HEIGHT DOWNSCALE(312)

	CertificateOfVaccination FindCertificateOfVaccination(const cv::Mat& inspection)
	{
		auto boundingBox = FindDocumentBoundingBox(inspection, CertificateOfVaccinationBorderColors, 2);

		cv::Mat canvas(cv::Size(CERTIFICATE_OF_VACCINATION_WIDTH, CERTIFICATE_OF_VACCINATION_HEIGHT), inspection.type(), CV_RGB(255, 255, 255));

		if (boundingBox.width == CERTIFICATE_OF_VACCINATION_WIDTH && boundingBox.height == CERTIFICATE_OF_VACCINATION_HEIGHT)
		{
			return CertificateOfVaccination{ inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height)) };
		}

		return CertificateOfVaccination(cv::Mat{});
	}

	cv::Mat PreprocessCertificateOfVaccination(const CertificateOfVaccination& certificateOfVaccination)
	{
		cv::Mat grayscale = ToGrayscale(certificateOfVaccination.mat);

		cv::Mat thresh;
		cv::threshold(grayscale, thresh, 95, 255, cv::THRESH_BINARY);

		return thresh;
	}

	CertificateOfVaccinationData GetCertificateOfVaccinationData(CertificateOfVaccination& cov)
	{
		auto binary = PreprocessCertificateOfVaccination(cov);

		auto name = GetFieldString(ExtractDocumentField(binary, cov.layoutProvider->GetNameBox()), DocumentType::CertificateOfVaccination);
		auto passportNumber = GetFieldString(ExtractDocumentField(binary, cov.layoutProvider->GetPassportNumberBox()), DocumentType::CertificateOfVaccination);
		auto vaccination1 = GetFieldString(ExtractDocumentField(binary, cov.layoutProvider->GetVaccination1Box()), DocumentType::CertificateOfVaccination);
		auto vaccination2 = GetFieldString(ExtractDocumentField(binary, cov.layoutProvider->GetVaccination2Box()), DocumentType::CertificateOfVaccination);
		auto vaccination3 = GetFieldString(ExtractDocumentField(binary, cov.layoutProvider->GetVaccination3Box()), DocumentType::CertificateOfVaccination);

		return CertificateOfVaccinationData{ name, passportNumber, vaccination1, vaccination2, vaccination3 };
	}

}