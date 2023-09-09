#pragma once
#include <string>
#include <memory>

#include <opencv2/opencv.hpp>

#include "base/color.h"
#include "base/common.h"
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

// Border Colors
namespace BorderColors {
	constexpr RgbColor Antegria{ 49, 77, 33 }; // 50 78 33
	constexpr RgbColor Arstotzka{ 59, 72, 59 }; // 60 72 59
	constexpr RgbColor Impor{ 102, 31, 9 }; //
	constexpr RgbColor Kolechia{ 85, 37, 63 }; // 83 35 64
	constexpr RgbColor Obristan{ 138, 12, 12 }; // 73 42 28
	constexpr RgbColor Republia{ 76, 42, 27 }; // 27 42 76
	constexpr RgbColor UnitedFederation{ 35, 30, 85 }; // 39 26 85
}

#define PASSPORT_WIDTH DOWNSCALE(260)
#define PASSPORT_HEIGHT DOWNSCALE(324)

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