#include "pch.h"
#include "base/ocr/ocr.h"

#include <climits>
#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>

#include "base/common.h"
#include "base/ocr/font.h"
#include "base/shape.h"

namespace paplease {
	namespace ocr {

		static inline int CharacterChecksum(const cv::Mat& character)
		{
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
			if (ch.rows * ch.cols >= 32)
			{
				std::cout << "ERROR CHARACTER TOO BIG \n";
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
			for (int i = 0; i < ch.rows; i++)
			{
				for (int j = 0; j < ch.cols; j++)
				{
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

#pragma region Load Font

		static constexpr char GetCharacterFromFilename(const std::string& filenameStem)
		{
			if (filenameStem == "QUESTION_MARK")
			{
				return '?';
			}
			return filenameStem.empty() ? '\0' : filenameStem[0];
		}

		static constexpr std::string_view GetFontPath(const FontInfo& fontInfo)
		{
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

		static std::unordered_map<int, char> LoadFontCharacters(const FontInfo& fontInfo)
		{
			std::unordered_map<int, char> chars{};
			std::string_view path = GetFontPath(fontInfo);
			for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
			{
				if (!entry.is_regular_file())
				{
					continue;
				}
#if DOWNSCALE_OPTIMIZATION
				auto character = cv::imread(entry.path().string(), cv::IMREAD_REDUCED_GRAYSCALE_2);
				// character = ScaleImage(character, 1.0f / 2.0f);
#else
				auto character = cv::imread(entry.path().string(), cv::IMREAD_UNCHANGED);
#endif

				int checksum = CharacterChecksum(character);
				chars[checksum] = GetCharacterFromFilename(entry.path().filename().stem().string());
			}
			return chars;
		}

		static std::unordered_map<int, char> GetLoadedFontCharacters(const FontInfo& fontInfo)
		{
			switch (fontInfo.typeface)
			{
				case Typeface::BM_Mini:
				{
					static const auto chars = LoadFontCharacters(fontInfo);
					return chars;
				}
				case Typeface::MiniKylie:
				{
					static const auto chars = LoadFontCharacters(fontInfo);
					return chars;
				}
				case Typeface::BoothNumber:
				{
					static const auto chars = LoadFontCharacters(fontInfo);
					return chars;
				}
				default:
				{
					static const auto chars = LoadFontCharacters(fontInfo);
					return chars;
				}
			}
		}

#pragma endregion

#pragma region Multiline Scan Helper

		static inline constexpr int FindTopLine(const cv::Mat& mat)
		{
			for (int row = 0; row < mat.rows; row++)
			{
				for (int col = 0; col < mat.cols; col++)
				{
					if (mat.at<uchar>(row, col)) continue;
					return row;
				}
			}
			return -1;
		}

		static inline constexpr int FindNextTopLine(const cv::Mat& mat, int previousLine, int fontSize)
		{
			for (int row = previousLine + fontSize; row < mat.rows; row++)
			{
				for (int col = 0; col < mat.cols; col++)
				{
					if (mat.at<uchar>(row, col)) continue;
					return row;
				}
			}
			return -1;
		}

#pragma endregion

#pragma region Text Boxing

		static std::vector<Rectangle> ImageToBoxesMultiline(const cv::Mat& mat, const FontInfo& fontInfo)
		{
			std::vector<Rectangle> boxes{};
			constexpr int maxAllowedWhitespace = 10;

			const int whitespaceSize = fontInfo.whitespaceSize;
			const int maxStepsFromLastCharacter = whitespaceSize * maxAllowedWhitespace;

			int lineTop = FindTopLine(mat);
			while (lineTop != -1)
			{
				int lastBoxX = INT_MAX;

				int left = -1;
				int top = -1;
				int width = -1;
				int height = -1;
				int bottom = -1;
				for (int col = 0; col < mat.cols; col++)
				{
					// check last character x position, if it's greater than max, cut scanner short
					if (col - lastBoxX > maxStepsFromLastCharacter) break;

					bool columnHasBlackPixel = false;
					const int bottomScan = std::min(lineTop + fontInfo.size, mat.rows);
					for (int row = lineTop; row < bottomScan; row++)
					{
						if (mat.at<uchar>(row, col)) continue;
						// We found a black pixel
						columnHasBlackPixel = true;
						if (left == -1) left = col;
						if (top == -1 || row < top) top = row;

						if (row > bottom && row < top + fontInfo.size) bottom = row;
					}

					if (!columnHasBlackPixel && left != -1 && top != -1)
					{
						lastBoxX = col;

						width = col - left;
						height = bottom - top + 1;
						// std::cout << "Width * Height: " << width * height << "\n";
						boxes.emplace_back(Rectangle{ left, top, width, height });
						left = -1;
						top = -1;
						width = -1;
						bottom = -1;
					}
				}
				lineTop = FindNextTopLine(mat, lineTop, fontInfo.size);
			}

			return boxes;
		}

		std::vector<Rectangle> ImageToBoxes(const cv::Mat& mat, const FontInfo& fontInfo)
		{
			// We'll assume that text is black, meaning has a pixel value of 0
			// Find top left most black pixel
			// Find complete bounding box of that

			// Add max whitespace after last character as a cut-off?

			if (mat.rows / fontInfo.size > 1)
			{
				return ImageToBoxesMultiline(mat, fontInfo);
			}

			std::vector<Rectangle> boxes{};

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

			return boxes;
		}

#pragma endregion

		std::string ImageToString(const cv::Mat& mat, const FontInfo& fontInfo)
		{
			std::vector<Rectangle> boxes = ImageToBoxes(mat, fontInfo);

			auto chars = GetLoadedFontCharacters(fontInfo); // temp fix

			const Rectangle* previousRectangle = nullptr;
			char previousChar = '\0';

			std::string field{};
			for (const auto& box : boxes)
			{
				cv::Mat character = mat(cv::Rect(box.x, box.y, box.width, box.height));
				int number = CharacterChecksum(character);

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

#if DOWNSCALE_OPTIMIZATION
				constexpr int sizeFactor = 1;
#else
				constexpr int sizeFactor = 2;
#endif

				if (previousRectangle)
				{ // add spaces
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

					if ((unsigned int)spaceSize >= (3 * sizeFactor) && previousChar != '1' && ch != '1')
					{ // space between two ones is same as a regular space
						int spaces = spaceSize < 0 ? 1 : spaceSize / (3 * sizeFactor);
						for (int i = 0; i < spaces; i++)
						{
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

	}  // namespace ocr
}  // namespace paplease
