#include "pch.h"
#include "base/ocr/ocr.h"

#include "base/common.h"
#include "base/image_process.h"
#include <string>
#include <cstring>

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
	while (lineTop != -1) {
		int top = -1;
		int left = -1;
		int width = -1;
		int bottom = -1;
		for (int col = 0; col < mat.cols; col++) {
			bool columnHasBlackPixel = false;
			for (int row = lineTop; row < lineTop + fontInfo.size && row < mat.rows; row++) {
				if (mat.at<uchar>(row, col)) continue;
				// We found a black pixel
				columnHasBlackPixel = true;
				if (left == -1) left = col;
				if (top == -1 || row < top) top = row;

				if (row > bottom) bottom = row;
			}

			if (!columnHasBlackPixel && left != -1 && top != -1) {
				width = col - left;

				boxes.emplace_back(Rectangle{ left, top, width, bottom - top + 1 });
				top = -1;
				left = -1;
				width = -1;
				bottom = -1;
			}
		}

		lineTop = FindNextTopLine(mat, lineTop, fontInfo.size);
	}

	return boxes;
}


std::vector<Rectangle> ImageToBoxes(const cv::Mat& mat, const FontInfo& fontInfo) {
	// We'll assume that text is black, meaning has a pixel value of 0
	// Find top left most black pixel
	// Find complete bounding box of that
	
	// Add max whitespace after last character as a cut-off?

	if (mat.rows / fontInfo.size > 1) {
		return ImageToBoxesMultiline(mat, fontInfo);
	}

	std::vector<Rectangle> boxes{};
#define USE_OLD_VERSION 0
#if USE_OLD_VERSION
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
#else

	int lastBoxX = INT_MAX;
	const int whitespaceSize = fontInfo.whitespaceSize; // depends on font
	constexpr int maxAllowedWhitespace = 10;
	const int maxStepsFromLastCharacter = whitespaceSize * maxAllowedWhitespace;

	int left = -1;
	int top = -1;
	int width = -1;
	int height = -1;
	int bottom = -1;
	for (int x = 0; x < mat.cols; x++)
	{
		// check last character x position, if it's greater than max, cut scanner short
		if (x - lastBoxX > maxStepsFromLastCharacter) break;

		bool columnHasBlackPixel = false;
		for (int y = 0; y < mat.rows; y++)
		{
			if (mat.at<uchar>(y, x)) continue;
			// We found a black pixel
			columnHasBlackPixel = true;
			if (left == -1) left = x;
			if (top == -1 || y < top) top = y;

			if (y > bottom && y < top + fontInfo.size) bottom = y;
		}

		if (!columnHasBlackPixel && left != -1 && top != -1)
		{
			lastBoxX = x;

			width = x - left;
			height = bottom - top + 1;
			// std::cout << "Width * Height: " << width * height << "\n";
			boxes.emplace_back(Rectangle{ left, top, width, height });
			left = -1;
			top = -1;
			width = -1;
			bottom = -1;
		}
	}
#endif
#undef USE_OLD_VERSION
	
	return boxes;
}

static inline int checksum_of_character(const cv::Mat& character) {
#if IS_DOWNSCALED
#if !OCR_CHAR_CHECKSUM_OPTIMIZATION
	cv::Mat ch = (character & 1) ^ 1;
#endif
#else
#if !OCR_CHAR_CHECKSUM_OPTIMIZATION
	cv::Mat ch = (ScaleImage(character, 1.0f / 2.0f) & 1) ^ 1;
#endif
#endif

#if OCR_CHAR_CHECKSUM_OPTIMIZATION
	const cv::Mat& ch = character;
#endif

#if 1
	if (ch.rows * ch.cols >= 32) {
		std::cout << "ERROR CHARACTER \n";
		//cv::imshow("Char", character);
		//cv::waitKey();
		return -1;
	}
#else
#if _DEBUG
	// assert(ch.rows * ch.cols < 32);
#endif
#endif

	int num = 0;
	for (int i = 0; i < ch.rows; i++) {
		for (int j = 0; j < ch.cols; j++) {
			auto c = ch.at<uchar>(i, j);
#if OCR_CHAR_CHECKSUM_OPTIMIZATION
			num = (num << 1) | ((c & 1) ^ 1); // concatenate binary digits row-wise
#else
			num = (num << 1) | c; // concatenate binary digits row-wise
#endif
		}
	}
	return num;
}

static constexpr std::string GetPathByFontInfo(const FontInfo& fontInfo) {
	switch (fontInfo.typeface)
	{
	case Typeface::Invalid:
		break;
	case Typeface::_04b03:
		return "C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\typefaces\\04b03\\14px";
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

static constexpr char GetCharacter(const std::string& str)
{
	if (str == "QUESTION_MARK")
	{
		return '?';
	}
	else
	{
		return str[0];
	}
}

static std::unordered_map<int, char> load_all_chars(const FontInfo& fontInfo) {
	std::unordered_map<int, char> chars{};
	std::string path = GetPathByFontInfo(fontInfo);
	for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
	{
		if (!entry.is_regular_file())
		{
			continue;
		}
#if DOWN_SCALE_OPTIMIZATION
		auto character = cv::imread(entry.path().string(), cv::IMREAD_REDUCED_GRAYSCALE_2);
		// character = ScaleImage(character, 1.0f / 2.0f);
#else
		auto character = cv::imread(entry.path().string(), cv::IMREAD_UNCHANGED);
#endif

		int checksum = checksum_of_character(character);
		chars[checksum] = GetCharacter(entry.path().filename().stem().string());
	}
	return chars;
}

static std::unordered_map<int, char> GetFontCharacters(const FontInfo& fontInfo)
{
	switch (fontInfo.typeface)
	{
		case Typeface::BM_Mini:
		{
			static const auto chars = load_all_chars(fontInfo);
			return chars;
		}
		case Typeface::MiniKylie:
		{
			static const auto chars = load_all_chars(fontInfo);
			return chars;
		}
		case Typeface::BoothNumber:
		{
			static const auto chars = load_all_chars(fontInfo);
			return chars;
		}
		default:
		{
			static const auto chars = load_all_chars(fontInfo);
			return chars;
		}
	}
}

std::string ImageToString(const cv::Mat& mat, const FontInfo& fontInfo) {
	std::vector<Rectangle> boxes = ImageToBoxes(mat, fontInfo);
	
	auto chars = GetFontCharacters(fontInfo); // temp fix

	const Rectangle* previousRectangle = nullptr;
	char previousChar = '\0';

	std::string field{};
	for (const auto& box : boxes) {
		cv::Mat character = mat(cv::Rect(box.x, box.y, box.width, box.height));
		int number = checksum_of_character(character);

		// unrecognizable character
		if (number <= 0 || chars.find(number) == chars.end())
		{
			std::cerr << "Unrecognized character\n";
#if 0
			cv::imshow("Char", mat);
			cv::waitKey(500);
#endif
			continue;
		}

		char ch = chars[number];

#if DOWN_SCALE_OPTIMIZATION
		constexpr int sizeFactor = 1;
#else
		constexpr int sizeFactor = 2;
#endif

		if (previousRectangle) { // add spaces
#if 1
			int horizontalSpaceFromLastBox = box.x - (previousRectangle->x + previousRectangle->width);
			const int letterSpaceSize = fontInfo.letterSpacingHorizontal * sizeFactor;
			const int whitespaceSize = fontInfo.whitespaceSize * sizeFactor;

			int whitespaces = ((horizontalSpaceFromLastBox - letterSpaceSize) / whitespaceSize);

			for (int i = 0; i < whitespaces; i++)
			{
				field.push_back(' ');
			}
#else 
			int spaceSize = box.x - (previousRectangle->x + previousRectangle->width);

			if ((unsigned int)spaceSize >= (3 * sizeFactor) && previousChar != '1' && ch != '1') { // space between two ones is same as a regular space
				int spaces = spaceSize < 0 ? 1 : spaceSize / (3 * sizeFactor);
				for (int i = 0; i < spaces; i++) {
					field.push_back(' ');
				}
			}
#endif
		}
		
		if (previousRectangle)
		{ // add newlines
			int verticalSpaceFromLastBox = box.y - (previousRectangle->y + previousRectangle->height);
			const int wrapSize = fontInfo.letterSpacingVertical * sizeFactor;
			const int newLineSize = fontInfo.newlineSize * sizeFactor;
			int newLines = ((verticalSpaceFromLastBox - wrapSize) / newLineSize);

			if (verticalSpaceFromLastBox - wrapSize >= 0)
			{ // add extra space when wrapping
				field.push_back(' ');
			}

			for (int i = 0; i < newLines; i++)
			{
				field.push_back('\n');
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
