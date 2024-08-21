#pragma once
#include <string>
#include <memory>

#include <opencv2/opencv.hpp>

#include "base/documents/document.h"
#include "base/layout.h"

// Passports:
// Antegria
// Arstotzka
// Impor
// Kolechia
// Obristan
// Republia
// United Federation

enum class PassportType {
	Invalid,
	Antegria,
	Arstotzka,
	Impor,
	Kolechia,
	Obristan,
	Republia,
	UnitedFederation
};


class Passport : public Document<PassportLayout> {
public:
	PassportType type;
	
	Passport(cv::Mat mat, PassportType type) : Document<PassportLayout>(mat, nullptr), type(type) {
		layoutProvider = CreatePassportLayout(type);
	}

private:
	std::unique_ptr<PassportLayout> CreatePassportLayout(PassportType type);
};

struct PassportData {
	std::string name;
	std::string dateOfBirth;
	std::string sex;
	std::string issuingCity;
	std::string expirationDate;
	std::string passportNumber;
	PassportType passportType;
};

Passport FindPassport(const cv::Mat& inspection);
PassportData GetPassportData(const Passport& passport);

cv::Mat PreprocessPassport(const Passport& passport);