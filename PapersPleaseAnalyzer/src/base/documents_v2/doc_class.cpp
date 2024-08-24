#pragma once
#include "pch.h"
#include "base/documents_v2/doc_class.h"

#include <unordered_set>

#include <opencv2/opencv.hpp>

#include "base/common.h"
#include "base/documents_v2/doc_appearance.h"
#include "base/image_process.h"

#include "base/documents/bounding_box_finder.inc"

namespace Documents::V2
{

Doc::Doc(const cv::Mat& mat, DocAppearance appearance, DocType docType, PassportType passportType)
	: m_mat{ mat }, m_appearance{ appearance }, m_docType{ docType }, m_passportType{ passportType }, m_docLayout{ DocLayout::Get(this->ToAppearanceType()) }
{}

#pragma region DocumentExtraction

#pragma region OutDated
#if 0
[[nodiscard]] static cv::Mat CutoutDocument(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance) {
	cv::Mat canvas(cv::Size(appearance.GetWidth(), appearance.GetHeight()), inspection.type(), CV_RGB(255, 255, 255));

	if (boundingBox.width == appearance.GetWidth() && boundingBox.height == appearance.GetHeight()) {
		return inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height));
	}

	return cv::Mat{};
}
#endif
#pragma endregion

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
			case RGB_VAL(DocAppearance::Get(AppearanceType::Passport_Antegria).GetColors()[0]):
				type = PassportType::Antegria;
				break;
			case RGB_VAL(DocAppearance::Get(AppearanceType::Passport_Arstotzka).GetColors()[0]):
				type = PassportType::Arstotzka;
				break;
			case RGB_VAL(DocAppearance::Get(AppearanceType::Passport_Impor).GetColors()[0]):
				type = PassportType::Impor;
				break;
			case RGB_VAL(DocAppearance::Get(AppearanceType::Passport_Kolechia).GetColors()[0]):
				type = PassportType::Kolechia;
				break;
			case RGB_VAL(DocAppearance::Get(AppearanceType::Passport_Obristan).GetColors()[0]):
				type = PassportType::Obristan;
				break;
			case RGB_VAL(DocAppearance::Get(AppearanceType::Passport_Republia).GetColors()[0]):
				type = PassportType::Republia;
				break;
			case RGB_VAL(DocAppearance::Get(AppearanceType::Passport_UnitedFederation).GetColors()[0]):
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

static std::vector<AppearanceType> FindAllDocumentsAppearanceType(const cv::Mat& inspection) {
	std::vector<AppearanceType> types{};
	std::unordered_set<AppearanceType> typesSet{};

	for (int y = 0; y < inspection.rows; y++) {
		const RgbColor* bgr = inspection.ptr<RgbColor>(y);
		for (int x = 0; x < inspection.cols; x++) {
			for (size_t i = 0; i < 17; i++) {
				auto* colors = DocAppearance::Get(static_cast<AppearanceType>(i)).GetColors();
				auto colorCount = DocAppearance::Get(static_cast<AppearanceType>(i)).GetColorCount();
				for (size_t i = 0; i < colorCount; i++) {
					if (BGR_VAL(bgr[x]) == RGB_VAL(colors[i]) && !typesSet.contains(static_cast<AppearanceType>(i))) {
						types.push_back(static_cast<AppearanceType>(i));
						typesSet.emplace(static_cast<AppearanceType>(i));
						goto DO_LOOP;
					}
				}
			}
		DO_LOOP:
			continue;
		
		}
	}
	return types;
}

static bool IsBorderColor(const cv::Mat& inspection, int x, int y, const DocAppearance& appearance) {
	RgbColor* bgr = (RgbColor*)inspection.ptr(y);

	bool isBorderColor = false;
	for (size_t i = 0; i < appearance.GetColorCount(); i++) {
		isBorderColor = BGR_VAL(bgr[x]) == RGB_VAL(appearance.GetColors()[i]);

		if (isBorderColor) {
			return isBorderColor;
		}
	}
	return false;

	switch (BGR_VAL(bgr[x])) {
	case RGB_VAL(DocAppearance::Get(AppearanceType::Passport_Antegria).GetColors()[0]):
	case RGB_VAL(DocAppearance::Get(AppearanceType::Passport_Arstotzka).GetColors()[0]):
	case RGB_VAL(DocAppearance::Get(AppearanceType::Passport_Impor).GetColors()[0]):
	case RGB_VAL(DocAppearance::Get(AppearanceType::Passport_Kolechia).GetColors()[0]):
	case RGB_VAL(DocAppearance::Get(AppearanceType::Passport_Obristan).GetColors()[0]):
	case RGB_VAL(DocAppearance::Get(AppearanceType::Passport_Republia).GetColors()[0]):
	case RGB_VAL(DocAppearance::Get(AppearanceType::Passport_UnitedFederation).GetColors()[0]):
		return true;
	default:
		return false;
	}
}

static bool IsCorner(const cv::Mat& inspection, const Rectangle& boundingBox, bool isTop, bool isLeft, const DocAppearance& appearance) {
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
		if (!IsBorderColor(inspection, xCorner + step * (-1 + 2 * isLeft), yCorner + step * (-1 + 2 * isTop), appearance)) {
			break;
		}
		step++;
	} while (true);

	// step will be +1, meaning if step is 0 then initial corner pixel did not have one of the border colors
	if (step == 0) {
		return false;
	}

	// step in x direction and y direction
	return IsBorderColor(inspection, xCorner + step * (-1 + 2 * isLeft), yCorner, appearance)
		&& IsBorderColor(inspection, xCorner, yCorner + step * (-1 + 2 * isTop), appearance);
}

cv::Mat CutoutDocumentProper(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance) {
	// Corner defintions
	// Top Left
	// A point that has color c and point to the right and below also has color c, while up and to the left does not have color c

	// If found bottom or top place image at bottom of white at x position (bottom - height)

	cv::Mat canvas(cv::Size(appearance.GetWidth(), appearance.GetHeight()), inspection.type(), CV_RGB(255, 255, 255));
	//cv::Mat canvas = cv::Mat::zeros(cv::Size(PASSPORT_WIDTH, PASSPORT_HEIGHT), inspection.type());
	if (appearance.GetType() == AppearanceType::Invalid) {
		std::cout << "INVALID\n";
		return canvas;
	}

	if (boundingBox.width == appearance.GetWidth() && boundingBox.height == appearance.GetHeight()) {
		return inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height));
	}

	cv::Mat cutDoc = inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height));

	/* Why find edge if we can just find corner?

	if (boundingBox.width == appearance.GetWidth()) { // Correct Width
		bool hasTopEdge = false;
		int top = (hasTopEdge) ? 0 : appearance.GetHeight() - cutDoc.cols;

	cv::Mat insetImage(canvas, cv::Rect(0, top, cutDoc.cols, cutDoc.rows));
		cutDoc.copyTo(insetImage);

	return canvas;
	}

	if (boundingBox.height == appearance.GetHeight()) { // Correct Height
		// IsCorner(canvas)
		bool hasLeftEdge = true;
		int left = (hasLeftEdge) ? 0 : appearance.GetWidth() - cutDoc.cols;

		cv::Mat insetImage(canvas, cv::Rect(left, 0, cutDoc.cols, cutDoc.rows));
		cutDoc.copyTo(insetImage);

		return canvas;
	}
	*/

	// If we can't see two corners try to find one

	if (IsCorner(inspection, boundingBox, true, true, appearance)) { // top left
		cv::Mat insetImage(canvas, cv::Rect(0, 0, cutDoc.cols, cutDoc.rows));
		cutDoc.copyTo(insetImage);

		return canvas;
	}
	else if (IsCorner(inspection, boundingBox, true, false, appearance)) { // top right
		cv::Mat insetImage(canvas, cv::Rect(appearance.GetWidth() - cutDoc.cols, 0, cutDoc.cols, cutDoc.rows));
		cutDoc.copyTo(insetImage);

		return canvas;
	}
	else if (IsCorner(inspection, boundingBox, false, true, appearance)) { // bottom left
		cv::Mat insetImage(canvas, cv::Rect(0, appearance.GetHeight() - cutDoc.rows, cutDoc.cols, cutDoc.rows));
		cutDoc.copyTo(insetImage);

		return canvas;
	}
	else if (IsCorner(inspection, boundingBox, false, false, appearance)) { // bottom right
		cv::Mat insetImage(canvas, cv::Rect(appearance.GetWidth() - cutDoc.cols, appearance.GetHeight() - cutDoc.rows, cutDoc.cols, cutDoc.rows));
		cutDoc.copyTo(insetImage);

		return canvas;
	}

	return canvas;
}

#pragma endregion

Doc FindDocument(const cv::Mat& inspection, DocType type) {
	if (type == DocType::Passport) {
		Rectangle boundingBox{};
		PassportType passportType = FindPassportTypeAndBoundingBox(inspection, boundingBox);
		auto appearance = DocAppearance::Get(static_cast<AppearanceType>(passportType));
		auto mat = CutoutDocumentProper(inspection, boundingBox, appearance);
		return Doc{ mat, appearance, type, passportType };
	}
	else {
		auto appearance = DocAppearance::Get(static_cast<AppearanceType>(type));
		auto boundingBox = FindDocumentBoundingBox(inspection, appearance.GetColors(), appearance.GetColorCount());

		if (!(boundingBox.width + boundingBox.height)) { // Invalid document
			return Doc{ cv::Mat{}, DocAppearance::Get(AppearanceType::Invalid), DocType::Invalid, PassportType::Invalid };
		}
		auto mat = CutoutDocumentProper(inspection, boundingBox, appearance);
		return Doc{ mat, appearance, type, PassportType::Invalid };

	}
}

std::vector<Doc> FindAllDocuments(const cv::Mat& inspection) {
	std::vector<Doc> documents{};
	
	for (const auto& type : FindAllDocumentsAppearanceType(inspection)) {
		documents.push_back(FindDocument(inspection, ((int)type > (int)DocType::Passport) ? DocType::Passport : (DocType)type));
	}

	return documents;
}

AppearanceType Doc::ToAppearanceType() const {
	if (m_docType != DocType::Passport) return static_cast<AppearanceType>(m_docType);
	return static_cast<AppearanceType>(m_passportType);
}

bool Doc::IsValid() const {
	return !m_mat.empty();
}

cv::Mat Doc::PreprocessDocument() const {
	if (m_docType == DocType::Invalid) return {};

	cv::Mat grayscale = ToGrayscale(m_mat);

	switch (this->ToAppearanceType())
	{
	case AppearanceType::AccessPermit:
	case AppearanceType::EntryPermit:
	case AppearanceType::EntryTicket:
	case AppearanceType::GrantOfAsylum:
	case AppearanceType::IdentitySupplement:
	case AppearanceType::WorkPass:
	case AppearanceType::Passport_Antegria:
	case AppearanceType::Passport_Arstotzka:
	case AppearanceType::Passport_Impor:
	case AppearanceType::Passport_Kolechia:
	case AppearanceType::Passport_Republia:
	case AppearanceType::Passport_UnitedFederation: {
		cv::Mat thresh;
		cv::threshold(grayscale, thresh, 127, 255, cv::THRESH_BINARY);
		return thresh;
	}
	case AppearanceType::CertificateOfVaccination: {
		cv::Mat thresh;
		cv::threshold(grayscale, thresh, 95, 255, cv::THRESH_BINARY);
		return thresh;
	}
	case AppearanceType::DiplomaticAuthorization: {
		cv::Mat thresh;
		cv::threshold(grayscale, thresh, 128, 255, cv::THRESH_BINARY);
		return thresh;
	}
	case AppearanceType::IdentityCard: {
		cv::Mat thresh;
		cv::threshold(grayscale, thresh, 127, 255, cv::THRESH_BINARY);

		cv::Mat insetImage(thresh, cv::Rect(DOWNSCALE(6), DOWNSCALE(20), DOWNSCALE(238), DOWNSCALE(14)));
		cv::Mat alter = (insetImage ^ 255);
		alter.copyTo(insetImage);
		return thresh;
	}
	case AppearanceType::Passport_Obristan: {
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
		throw "Something is very wrong!";
	}
}


#pragma region Preprocessing
#if 0
cv::Mat PreprocessAccessPermit(const AccessPermit& accessPermit) {
	cv::Mat grayscale = ToGrayscale(accessPermit.mat);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, 127, 255, cv::THRESH_BINARY);

	return thresh;
}

cv::Mat PreprocessEntryPermit(const EntryPermit& entryPermit) {
	cv::Mat grayscale = ToGrayscale(entryPermit.mat);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, 127, 255, cv::THRESH_BINARY);

	return thresh;
}

cv::Mat PreprocessEntryTicket(const EntryTicket& entryTicket) {
	cv::Mat grayscale = ToGrayscale(entryTicket.mat);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, 127, 255, cv::THRESH_BINARY);

	return thresh;
}

cv::Mat PreprocessGrantOfAsylum(const GrantOfAsylum& goa) {
	cv::Mat grayscale;
	cv::cvtColor(goa.mat, grayscale, cv::COLOR_BGR2GRAY);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, 127, 255, cv::THRESH_BINARY);

	return thresh;
}

cv::Mat PreprocessIdentitySupplement(const IdentitySupplement& identitySupplement) {
	cv::Mat grayscale = ToGrayscale(identitySupplement.mat);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, 127, 255, cv::THRESH_BINARY);

	return thresh;
}

cv::Mat PreprocessWorkPass(const WorkPass& workPass) {
	cv::Mat grayscale = ToGrayscale(workPass.mat);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, 127, 255, cv::THRESH_BINARY);

	return thresh;
}

cv::Mat PreprocessCertificateOfVaccination(const CertificateOfVaccination& certificateOfVaccination) {
	cv::Mat grayscale = ToGrayscale(certificateOfVaccination.mat);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, 95, 255, cv::THRESH_BINARY);

	return thresh;
}

cv::Mat PreprocessDiplomaticAuthorization(const DiplomaticAuthorization& diplomaticAuthorization) {
	cv::Mat grayscale = ToGrayscale(diplomaticAuthorization.mat);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, 128, 255, cv::THRESH_BINARY);
	return thresh;
}

cv::Mat PreprocessIdentityCard(const IdentityCard& identityCard) {
	cv::Mat grayscale = ToGrayscale(identityCard.mat);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, 127, 255, cv::THRESH_BINARY);

	cv::Mat insetImage(thresh, cv::Rect(DOWNSCALE(6), DOWNSCALE(20), DOWNSCALE(238), DOWNSCALE(14)));
	cv::Mat alter = (insetImage ^ 255);
	alter.copyTo(insetImage);

	return thresh;
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


#endif
#pragma endregion

const DocLayout const* const Doc::GetLayout() const noexcept {
	return &m_docLayout;
}

DocData Doc::GetDocumentData() const {
	if (!this->IsValid()) return {};

	auto binary = this->PreprocessDocument();
	auto layout = m_docLayout;
	auto layouts = layout.GetAllLayouts();

	std::array<Data, DocData::ARRAY_LENGTH> fields{};

	//size_t count = 0;
	for (size_t i = 0; i < layout.GetLayoutCount(); i++) {
		if (layouts[i].GetType() == DataFieldType::TextField) {
			auto string = GetFieldString(ExtractDocumentField(binary, layouts[i].GetBox()), m_docType);
			
			// fields[count] = Data{ string, layouts[i].GetCategory() };
			auto position = static_cast<size_t>(layouts[i].GetCategory());
			fields[position] = Data{string, layouts[i].GetCategory()};
			// count++;
		}
		else {
			auto position = static_cast<size_t>(layouts[i].GetCategory());
			fields[position] = Data{ "", layouts[i].GetCategory() };
		}
	}

	// return { fields, count };
	return { fields };
}

}