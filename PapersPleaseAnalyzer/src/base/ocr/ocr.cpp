#include "pch.h"
#include "base/ocr/ocr.h"

#include "base/common.h"


static int FindTopLine(const cv::Mat& mat) {
	int top = -1;
	for (int y = 0; y < mat.rows; y++) {
		for (int x = 0; x < mat.cols; x++) {
			if (mat.at<uchar>(y, x)) continue;
			if (top == -1) top = y;
		}
		if (top != -1) break;
	}
	return top;
}

static int FindNextTopLine(const cv::Mat& mat, int previousLine, int fontSize) {
	int top = -1;
	for (int y = previousLine + fontSize; y < mat.rows; y++) {
		for (int x = 0; x < mat.cols; x++) {
			if (mat.at<uchar>(y, x)) continue;
			if (top == -1) top = y;
		}
		if (top != -1) break;
	}
	return top;
}

static std::vector<Rectangle> ImageToBoxesMultiline(const cv::Mat& mat, const FontInfo& fontInfo) {
	std::vector<Rectangle> boxes{};
	
	cv::Mat remainMat = mat;

	int lineTop = FindTopLine(mat);
	do {
		int top = -1;
		int left = -1;
		int width = -1;
		int bottom = -1;
		for (int x = 0; x < mat.cols; x++) {
			bool columnHasBlackPixel = false;
			for (int y = lineTop; y < lineTop + fontInfo.size && y < mat.rows; y++) {
				if (mat.at<uchar>(y, x)) continue;
				// We found a black pixel
				columnHasBlackPixel = true;
				if (left == -1) left = x;
				if (top == -1 || y < top) top = y;

				if (y > bottom) bottom = y;
			}

			if (!columnHasBlackPixel && left != -1 && top != -1) {
				width = x - left;

				boxes.emplace_back(Rectangle{ left, top, width, bottom - top + 1 });
				top = -1;
				left = -1;
				width = -1;
				bottom = -1;
			}
		}

		lineTop = FindNextTopLine(mat, lineTop, fontInfo.size);
	} while (lineTop != -1);

	return boxes;
}


std::vector<Rectangle> ImageToBoxes(const cv::Mat& mat, const FontInfo& fontInfo) {
	// We'll assume that text is black, meaning has a pixel value of 0
	// Find top left most black pixel
	// Find complete bounding box of that

	if (mat.rows / fontInfo.size > 1) {
		return ImageToBoxesMultiline(mat, fontInfo);
	}

	std::vector<Rectangle> boxes{};

	int left = -1;
	int top = -1;
	int width = -1;
	int bottom = -1;
	for (int x = 0; x < mat.cols; x++) {
		bool columnHasBlackPixel = false;
		for (int y = 0; y < mat.rows; y++) {
			if (mat.at<uchar>(y, x)) continue;
			// We found a black pixel
			columnHasBlackPixel = true;
			if (left == -1) left = x;
			if (top == -1 || y < top) top = y;

			if (y > bottom && y < top + fontInfo.size) bottom = y;
		}

		if (!columnHasBlackPixel && left != -1 && top != -1) {
			width = x - left;

			boxes.emplace_back(Rectangle{ left, top, width, bottom - top + 1 });
			left = -1;
			top = -1;
			width = -1;
			bottom = -1;
		}
	}
	
	return boxes;
}

static int checksum_of_character(const cv::Mat& character) {
	cv::Mat ch = (DownScaleImage(character, 2.0f) & 1) ^ 1;

	if (ch.rows * ch.cols >= 32) {
		std::cout << "\n";
		cv::imshow("Char", character);
		cv::waitKey();
	}
	assert(ch.rows * ch.cols < 32);


	int num = 0;
	for (int i = 0; i < ch.rows; i++) {
		for (int j = 0; j < ch.cols; j++) {
			int c = ch.at<uchar>(i, j);
			num = (num << 1) | c; // concatenate binary digits row-wise
		}
	}
	return num;
}

static std::string GetPathByFontInfo(const FontInfo& fontInfo) {
	switch (fontInfo.typeface)
	{
	case Typeface::Invalid:
		break;
	case Typeface::BM_Mini:
		return "C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\typefaces\\BM Mini\\16px";
	case Typeface::MiniKylie:
		return "C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\typefaces\\MiniKylie\\12px";
	case Typeface::BoothNumber:
		return "C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\typefaces\\BoothNumber\\12px";
	default:
		break;
	}

	return "";
}

static std::unordered_map<int, char> load_all_chars(const FontInfo& fontInfo) {
	std::string path = GetPathByFontInfo(fontInfo);

	std::unordered_map<int, char> chars{};
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_directory()) {
			for (const auto& entry1 : std::filesystem::directory_iterator(entry.path())) {
				auto character = cv::imread(entry1.path().generic_string(), cv::IMREAD_UNCHANGED);

				int sum = checksum_of_character(character);
				chars[sum] = entry1.path().filename().string()[0];
			}
		}
		else {
			auto character = cv::imread(entry.path().generic_string(), cv::IMREAD_UNCHANGED);

			int sum = checksum_of_character(character);
			chars[sum] = entry.path().filename().string()[0];
		}
	}
	return chars;
}

std::string ImageToString(const cv::Mat& mat, const FontInfo& fontInfo) {
	std::vector<Rectangle> boxes = ImageToBoxes(mat, fontInfo);

	auto chars = load_all_chars(fontInfo);

	const Rectangle* previousRectangle = nullptr;
	char previousChar = '\0';

	std::string field{};
	for (const auto& box : boxes) {
		cv::Mat character = mat(cv::Rect(box.x, box.y, box.width, box.height));
		int number = checksum_of_character(character);
		char ch = chars[number];

		if (previousRectangle) { // add spaces
			int spaceSize = box.x - (previousRectangle->x + previousRectangle->width);

			if ((unsigned int)spaceSize >= 6 && previousChar != '1' && ch != '1') { // space between two ones is same as a regular space
				int spaces = spaceSize < 0 ? 1 : spaceSize / 6;
				for (int i = 0; i < spaces; i++) {
					field.push_back(' ');
				}
			}
		}
		previousRectangle = &box;
		previousChar = ch;

		field.push_back(ch);
	}

	return field;
}

// text detection
// draw bounding boxes around every linked pixels of same color, whitespace shall be what separates two characters
// find a way to recognize or create a ruleset describing vertical spacing of fonts

// Letter font consists of three parts:
// 1. Main body which seems to be regular sized spaced
// 2. Top, which is used by letters such as i and capital letters such as K
// 3. Bottom, which is used by letters such as y, j

// Digits seem to be 4 wide, even the 1
// Letter wideness varies
