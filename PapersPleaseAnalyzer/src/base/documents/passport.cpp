#include "pch.h"
#include "base/documents/passport.h"

#include "base/image_process.h"
#include "base/shape.h"

static PassportType FindPassportTypeAndBoundingBox(const cv::Mat& inspection, Rectangle& outBoundingBox) {
	PassportType type = PassportType::Invalid;

	int left = INT_MAX;
	int top = INT_MAX;
	int right = 0;
	int bottom = 0;

	for (int y = 0; y < inspection.rows; y++) {
		const RgbColor* bgr = inspection.ptr<RgbColor>(y);

		int min_x = INT_MAX;
		int max_x = 0;
		for (int x = 0; x < inspection.cols; x++) {
			switch (BGR_VAL(bgr[x])) {
			case RGB_VAL(BorderColors::Antegria):
				type = PassportType::Antegria;
				break;
			case RGB_VAL(BorderColors::Arstotzka):
				type = PassportType::Arstotzka;
				break;
			case RGB_VAL(BorderColors::Impor):
				type = PassportType::Impor;
				break;
			case RGB_VAL(BorderColors::Kolechia):
				type = PassportType::Kolechia;
				break;
			case RGB_VAL(BorderColors::Obristan):
				type = PassportType::Obristan;
				break;
			case RGB_VAL(BorderColors::Republia):
				type = PassportType::Republia;
				break;
			case RGB_VAL(BorderColors::UnitedFederation):
				type = PassportType::UnitedFederation;
				break;
			default:
				continue;
			}
			if (x < min_x) min_x = x;
			if (x > max_x) max_x = x;
			if (y < top) top = y;
			if (y > bottom) bottom = y;
		}
		if (max_x) {
			if (min_x < left) left = min_x;
			if (max_x > right) right = max_x;
		}
	}

	outBoundingBox = Rectangle{ left, top, right - left + 1, bottom - top + 1 };

	return type;
}

static bool IsBorderColor(const cv::Mat& inspection, int x, int y) {
	RgbColor* bgr = (RgbColor*)inspection.ptr(y);
	switch (BGR_VAL(bgr[x])) {
	case RGB_VAL(BorderColors::Antegria):
	case RGB_VAL(BorderColors::Arstotzka):
	case RGB_VAL(BorderColors::Impor):
	case RGB_VAL(BorderColors::Kolechia):
	case RGB_VAL(BorderColors::Obristan):
	case RGB_VAL(BorderColors::Republia):
	case RGB_VAL(BorderColors::UnitedFederation):
		return true;
	default:
		return false;
	}
}

static bool IsCorner(const cv::Mat& inspection, const Rectangle& boundingBox, bool isTop, bool isLeft) {
	// if isTop is true, then we check down, else check up
	// if isLeft is true, then we check right, else check left

	int xCorner = boundingBox.x + (boundingBox.width - 1) * !isLeft;
	int yCorner = boundingBox.y + (boundingBox.height - 1) * !isTop;
	int width = boundingBox.width;
	int height = boundingBox.height;

	// Start by stepping closest to the inner corner
	int step = 0;
	do {
		if (step >= height || step >= width) {
			break;
		}
		// (-1 + 2 * bool) yields values -1 or 1
		if (!IsBorderColor(inspection, xCorner + step * (-1 + 2 * isLeft), yCorner + step * (- 1 + 2 * isTop))) {
			break;
		}
		step++;
	} while (true);

	// step will be +1, meaning if step is 0 then initial corner pixel did not have one of the border colors
	if (step == 0) {
		return false;
	}

	// step in x direction and y direction
	return IsBorderColor(inspection, xCorner + step * (-1 + 2 * isLeft), yCorner) && IsBorderColor(inspection, xCorner, yCorner + step * (-1 + 2 * isTop));
}

Passport FindPassport(const cv::Mat& inspection) {
	// Corner defintions
	// Top Left
	// A point that has color c and point to the right and below also has color c, while up and to the left does not have color c
	Rectangle boundingBox;
	PassportType type = FindPassportTypeAndBoundingBox(inspection, boundingBox);

	// If found bottom or top place image at bottom of white at x position (bottom - height)

	cv::Mat canvas(cv::Size(PASSPORT_WIDTH, PASSPORT_HEIGHT), inspection.type(), CV_RGB(255, 255, 255));
	//cv::Mat canvas = cv::Mat::zeros(cv::Size(PASSPORT_WIDTH, PASSPORT_HEIGHT), inspection.type());
	if (type == PassportType::Invalid) {
		std::cout << "INVALID\n";
		return Passport{ canvas, type };
	}

	if (boundingBox.width == PASSPORT_WIDTH && boundingBox.height == PASSPORT_HEIGHT) {
		return Passport{ inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height)), type };
	}

	cv::Mat cutPassport = inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height));

	if (boundingBox.width == PASSPORT_WIDTH) {
		cv::Mat insetImage(canvas, cv::Rect(0, PASSPORT_HEIGHT - cutPassport.rows, cutPassport.cols, cutPassport.rows));
		cutPassport.copyTo(insetImage);

		return Passport{ canvas, type };
	}

	if (boundingBox.height == PASSPORT_HEIGHT) {
		cv::Mat insetImage(canvas, cv::Rect(PASSPORT_WIDTH - cutPassport.cols, 0, cutPassport.cols, cutPassport.rows));
		cutPassport.copyTo(insetImage);

		return Passport{ canvas, type };
	}

	// If we can't see two corners try to find one

	if (IsCorner(inspection, boundingBox, true, true)) { // top left
		cv::Mat insetImage(canvas, cv::Rect(0, 0, cutPassport.cols, cutPassport.rows));
		cutPassport.copyTo(insetImage);

		return Passport{ canvas, type };
	}
	else if (IsCorner(inspection, boundingBox, true, false)) { // top right
		cv::Mat insetImage(canvas, cv::Rect(PASSPORT_WIDTH - cutPassport.cols, 0, cutPassport.cols, cutPassport.rows));
		cutPassport.copyTo(insetImage);

		return Passport{ canvas, type };
	}
	else if (IsCorner(inspection, boundingBox, false, true)) { // bottom left
		cv::Mat insetImage(canvas, cv::Rect(0, PASSPORT_HEIGHT - cutPassport.rows, cutPassport.cols, cutPassport.rows));
		cutPassport.copyTo(insetImage);

		return Passport{ canvas, type };
	}
	else if (IsCorner(inspection, boundingBox, false, false)) { // bottom right
		cv::Mat insetImage(canvas, cv::Rect(PASSPORT_WIDTH - cutPassport.cols, PASSPORT_HEIGHT - cutPassport.rows, cutPassport.cols, cutPassport.rows));
		cutPassport.copyTo(insetImage);

		return Passport{ canvas, type };
	}

	return Passport{ canvas, type };
}

std::unique_ptr<PassportLayout> Passport::CreatePassportLayout(PassportType t) {
	switch (t)
	{
	case PassportType::Invalid:
		return nullptr;
	case PassportType::Antegria:
		return std::make_unique<AntegriaPassportLayout>();
	case PassportType::Arstotzka:
		return std::make_unique<ArstotzkaPassportLayout>();
	case PassportType::Impor:
		return std::make_unique<ImporPassportLayout>();
	case PassportType::Kolechia:
		return std::make_unique<KolechiaPassportLayout>();
	case PassportType::Obristan:
		return std::make_unique<ObristanPassportLayout>();
	case PassportType::Republia:
		return std::make_unique<RepubliaPassportLayout>();
	case PassportType::UnitedFederation:
		return std::make_unique<UnitedFederationPassportLayout>();
	default:
		return nullptr;
	}
}

cv::Mat PreprocessPassport(const Passport& passport) {
	cv::Mat grayscale;
	cv::cvtColor(passport.mat, grayscale, cv::COLOR_BGR2GRAY);

	switch (passport.type) {
	case PassportType::Invalid:
		return grayscale;
	case PassportType::Antegria:
	case PassportType::Arstotzka:
	case PassportType::Impor:
	case PassportType::Kolechia:
	case PassportType::Republia:
	case PassportType::UnitedFederation: {
		cv::Mat thresh;
		cv::threshold(grayscale, thresh, 127, 255, cv::THRESH_BINARY);
		return thresh;
	}
	case PassportType::Obristan: {
		cv::Mat thresh2;
		cv::threshold(grayscale, thresh2, 226, 255, cv::THRESH_BINARY);

		cv::Mat insetimage(thresh2, cv::Rect(DOWNSCALE(14), DOWNSCALE(192), DOWNSCALE(236), DOWNSCALE(20)));
		cv::Mat alter = insetimage ^ 255;
		alter.copyTo(insetimage);

		cv::Mat thresh3;
		cv::threshold(grayscale, thresh3, 76, 255, cv::THRESH_BINARY_INV);

		return (thresh2 | thresh3) ^ 255;
	}
	default:
		return grayscale;
	}
}



PassportData GetPassportData(const Passport& passport) {
	if (passport.type == PassportType::Invalid) return PassportData{};

	auto binaryPassport = PreprocessPassport(passport);

	auto name = GetFieldString(ExtractDocumentField(binaryPassport, passport.layoutProvider->GetNameBox()), DocumentType::Passport);
	auto dob = GetFieldString(ExtractDocumentField(binaryPassport, passport.layoutProvider->GetDateOfBirthBox()), DocumentType::Passport);
	auto sex = GetFieldString(ExtractDocumentField(binaryPassport, passport.layoutProvider->GetSexBox()), DocumentType::Passport);
	auto iss = GetFieldString(ExtractDocumentField(binaryPassport, passport.layoutProvider->GetIssuingCityBox()), DocumentType::Passport);
	auto expirationDate = GetFieldString(ExtractDocumentField(binaryPassport, passport.layoutProvider->GetExpirationDateBox()), DocumentType::Passport);
	auto passportNumber = GetFieldString(ExtractDocumentField(binaryPassport, passport.layoutProvider->GetPassportNumberBox()), DocumentType::Passport);

	return PassportData{ name, dob, sex, iss, expirationDate, passportNumber, passport.type };
}