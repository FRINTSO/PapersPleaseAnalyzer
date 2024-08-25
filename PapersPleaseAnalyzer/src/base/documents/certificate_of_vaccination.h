#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "base/documents/document.h"
#include "base/documents/layout.h"

namespace Documents::V1 {

	class CertificateOfVaccination : public Document<CertificateOfVaccinationLayout>
	{
	public:
		using Document<CertificateOfVaccinationLayout>::Document;
	};

	struct CertificateOfVaccinationData
	{
		std::string name;
		std::string passportNumber;
		std::string vaccination1;
		std::string vaccination2;
		std::string vaccination3;
	};

	CertificateOfVaccination FindCertificateOfVaccination(const cv::Mat& inspection);
	CertificateOfVaccinationData GetCertificateOfVaccinationData(CertificateOfVaccination& cov);

	cv::Mat PreprocessCertificateOfVaccination(const CertificateOfVaccination& certificateOfVaccination);

}