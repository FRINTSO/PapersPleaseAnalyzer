#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "base/color.h"
#include "base/common.h"
#include "base/documents/document.h"
#include "base/layout.h"


namespace {
	constexpr RgbColor CertificateOfVaccinationBorderColors[] = {
		{ 234, 223, 128 },
		{ 177, 151, 97 },
	};
}

#define CERTIFICATE_OF_VACCINATION_WIDTH DOWNSCALE(270)
#define CERTIFICATE_OF_VACCINATION_HEIGHT DOWNSCALE(312)

class CertificateOfVaccination : public Document<CertificateOfVaccinationLayout> {
public:
	using Document<CertificateOfVaccinationLayout>::Document;
};

struct CertificateOfVaccinationData {
	std::string name;
	std::string passportNumber;
	std::string vaccination1;
	std::string vaccination2;
	std::string vaccination3;
};

CertificateOfVaccination FindCertificateOfVaccination(const cv::Mat& inspection);
CertificateOfVaccinationData GetCertificateOfVaccinationData(CertificateOfVaccination& cov);

cv::Mat PreprocessCertificateOfVaccination(const CertificateOfVaccination& certificateOfVaccination);