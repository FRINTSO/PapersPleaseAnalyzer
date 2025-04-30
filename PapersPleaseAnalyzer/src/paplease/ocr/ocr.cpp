#include "pch.h"
#include "paplease/ocr/ocr.h"

#include <climits>
#include <filesystem>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>

#include "paplease/common/common.h"
#include "paplease/common/image_process.h"
#include "paplease/ocr/font.h"

namespace paplease {
	namespace ocr {

		static inline int CharacterChecksum(const cv::Mat& character)
		{
#if IS_DOWNSCALED
	#if not OCR_CHAR_CHECKSUM_OPTIMIZATION
			cv::Mat ch = (character & 1) ^1;
	#else
			const cv::Mat& ch = character;
	#endif
#else
			cv::Mat ch;
			if (character.rows / 2 >= 1 && character.cols / 2 >= 1)
			{
				ch = ScaleImage(character, SCALE);
			}
			else
			{
				ch = character;
			}
	#if not OPTIMIZE_OCR_CHAR_CHECKSUM
			ch = (ch & 1) ^ 1;
	#endif
#endif

#if 1
			if (ch.rows * ch.cols >= 32)
			{
				// LOG_ERR("CHARACTER TOO BIG");
				//cv::imshow("Char", character);
				//cv::waitKey();
				return -1;
			}
#else
			assert(ch.rows * ch.cols < 32);
#endif

			int num = 0;
			for (int i = 0; i < ch.rows; i++)
			{
				for (int j = 0; j < ch.cols; j++)
				{
					auto c = ch.at<uchar>(i, j);
#if OPTIMIZE_OCR_CHAR_CHECKSUM
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
				auto character = cv::imread(entry.path().string(), cv::IMREAD_UNCHANGED);
#if OPTIMIZE_DOWNSCALE
				character = ScaleImage(character, 1.0f / 2.0f);
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

				int left = INT_MAX;
				int top = INT_MAX;
				int right = 0;
				int bottom = 0;
				for (int x = 0; x < mat.cols; x++)
				{
					// check last character x position, if it's greater than max, cut scanner short
					if (x - lastBoxX > maxStepsFromLastCharacter) break;

					bool columnHasBlackPixel = false;
					const int bottomOfLine = std::min(lineTop + fontInfo.size, mat.rows);
					for (int y = lineTop; y < bottomOfLine; y++)
					{
						if (mat.at<uchar>(y, x)) continue;
						// We found a black pixel
						columnHasBlackPixel = true;

						top = std::min(top, y);
						bottom = std::max(bottom, y);
					}

					if (columnHasBlackPixel)
					{
						left = std::min(left, x);
						right = std::max(right, x);
					}
					else if (left != INT_MAX)
					{
						lastBoxX = x;

						int width = right - left + 1;
						int height = bottom - top + 1;
						boxes.emplace_back(Rectangle{ left, top, width, height });
						left = INT_MAX;
						top = INT_MAX;
						width = 0;
						bottom = 0;
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

			int left = INT_MAX;
			int top = INT_MAX;
			int right = 0;
			int bottom = 0;
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

					top = std::min(top, y);
					bottom = std::max(bottom, y);
				}

				if (columnHasBlackPixel)
				{
					left = std::min(left, x);
					right = std::max(right, x);
				}
				else if (left != INT_MAX)
				{
					lastBoxX = x;

					int width = right - left + 1;
					int height = bottom - top + 1;
					boxes.emplace_back(Rectangle{ left, top, width, height });
					left = INT_MAX;
					top = INT_MAX;
					width = 0;
					bottom = 0;
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
					//LOG_ERR("Unrecognized character");
					continue;
				}

				char ch = chars[number];

				if (previousRectangle)
				{ // add spaces
					int horizontalSpaceFromLastBox = box.x - (previousRectangle->x + previousRectangle->width);
					const int letterSpaceSize = fontInfo.letterSpacingHorizontal;
					const int whitespaceSize = fontInfo.whitespaceSize;

					int whitespaces = ((horizontalSpaceFromLastBox - letterSpaceSize) / whitespaceSize);

					if (!(whitespaces >= 1 && ch == '1' && field.back() == '1'))
					{
						if (whitespaces > 0)
							field.append(whitespaces, ' ');
					}
				}

				if (previousRectangle)
				{ // add newlines
					int verticalSpaceFromLastBox = box.y - (previousRectangle->y + previousRectangle->height);
					const int wrapSize = fontInfo.letterSpacingVertical;
					const int newLineSize = fontInfo.newlineSize;
					int newLines = ((verticalSpaceFromLastBox - wrapSize) / newLineSize);

					if (verticalSpaceFromLastBox - wrapSize >= 0)
					{ // add extra space when wrapping
						field.push_back(' ');
					}

					if (newLines > 0)
						field.append(newLines, '\n');
				}

				previousRectangle = &box;
				previousChar = ch;

				field.push_back(ch);
			}

			return field;
		}

	}  // namespace ocr
}  // namespace paplease
