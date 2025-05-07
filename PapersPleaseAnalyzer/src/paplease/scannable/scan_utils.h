#pragma once
#include "paplease/common/shape.h"
#include "paplease/core/enum_base.h"
#include "paplease/types.h"

namespace paplease {
    namespace scannable {
		namespace scan_utils {

			using PixelMatchCondition = const std::function<bool(int, int)>&;

			static inline constexpr int TryFindBlackPixel(const cv::Mat& mat, int row)
			{
				for (int col = 0; col < mat.cols; col++)
				{
					if (!mat.at<bool>(row, col))
					{ // isBlackPixel
						return col;
					}
				}
				return -1;
			}

			static inline constexpr int CountContinuousBlackPixelsCol(const cv::Mat& mat, int row, int left)
			{
				int count = 0;
				for (int col = left; col < mat.cols; col++)
				{
					if (!mat.at<bool>(row, col))
					{ // isBlackPixel
						count++;
					}
					else
					{
						break;
					}
				}
				return count;
			}

			static inline constexpr int CountContinuousBlackPixelsRow(const cv::Mat& mat, int top, int col)
			{
				int count = 0;
				for (int row = top; row < mat.rows; row++)
				{
					if (!mat.at<bool>(row, col))
					{ // isBlackPixel
						count++;
					}
					else
					{
						break;
					}
				}
				return count;
			}

			static inline constexpr int FindValidRightEdge(const cv::Mat& transcript, int row, int left, int minColLimit)
			{
				int colCount = CountContinuousBlackPixelsCol(transcript, row, left);
				return (colCount >= minColLimit) ? left + colCount - 1 : -1;
			}

			static inline constexpr int FindValidBottomEdge(const cv::Mat& transcript, int top, int right, int minRowLimit)
			{
				int rowCount = CountContinuousBlackPixelsRow(transcript, top, right);
				return (rowCount >= minRowLimit) ? top + rowCount - 1 : -1;
			}

			static void InvertRegion(cv::Mat& transcript, int left, int top, int right, int bottom)
			{
				cv::Mat cutout(transcript, cv::Rect(left, top, right - left + 1, bottom - top + 1));
				cv::bitwise_not(cutout, cutout);
			}

			struct Corner : public core::EnumBase<u8>
			{
				using EnumBase::EnumBase;
				enum : u8
				{
					//              xy
					TopLeft = 0b00,
					TopRight = 0b10,
					BottomLeft = 0b01,
					BottomRight = 0b11,
				};
			};

			class Direction : public core::EnumBase<u8>
			{
			public:
				using EnumBase::EnumBase;
				enum : u8
				{
					// First bit is xy direction, x = 0, y = 1
					// Second bit is plus minus (increasing or decreasing), + = 0, - = 1 (because 1 is treated as a signed integer)
					//     +/-  x/y
					Up = 0b11,
					Down = 0b01,
					Left = 0b10,
					Right = 0b00,
				};

				inline constexpr bool IsHorizontal() const noexcept { return ((*this & Direction::AxisMask) == Horizontal); }
				inline constexpr bool IsVertical() const noexcept { return ((*this & Direction::AxisMask) == Vertical); }
				inline constexpr int StepX() const noexcept { return StepSize() * IsHorizontal(); }
				inline constexpr int StepY() const noexcept { return StepSize() * IsVertical(); }

			private:
				inline constexpr int StepSize() const noexcept { return 1 - 2 * ((*this & Direction::SignMask) != 0); }

				constexpr static u8 AxisMask = 0b01; // 0 = X, 1 = Y
				constexpr static u8 SignMask = 0b10; // 0 = +, 1 = -
				constexpr static u8 Horizontal = 0;
				constexpr static u8 Vertical = 1;
			};

			struct ScanDirection : core::EnumBase<u8>
			{
				using EnumBase::EnumBase;
				enum : u8
				{
					Horizontal,
					Vertical,
				};
			};

			static inline constexpr Point FindCorner(Corner corner, const cv::Mat& mat, PixelMatchCondition whatPixelShouldMatch, ScanDirection scanDirection = ScanDirection::Horizontal)
			{
				constexpr u8 verticalMask = 0b01;
				constexpr u8 horizontalMask = 0b10;
				// start pos
				// if top -> y = 0
				// if bottom -> y = mat.rows
				// if left -> x = 0
				// if right -> x = mat.cols
				Point startPos{ (mat.cols - 1) * ((corner & horizontalMask) != 0), (mat.rows - 1) * ((corner & verticalMask) != 0) };

				// scan direction
				// if top -> y++
				// if bottom -> y--
				// if left -> x++
				// if right -> x--
				Point stepDir{ 1 + -2 * ((corner & horizontalMask) != 0), 1 + -2 * ((corner & verticalMask) != 0) };

				if (scanDirection == ScanDirection::Horizontal)
				{
					for (int row = startPos.y; 0 <= row && row < mat.rows; row += stepDir.y)
					{
						for (int column = startPos.x; 0 <= column && column < mat.cols; column += stepDir.x)
						{
							if (whatPixelShouldMatch(row, column))
							{
								return { column, row };
							}
						}
					}
				}
				else
				{
					for (int column = startPos.x; 0 <= column && column < mat.cols; column += stepDir.x)
					{
						for (int row = startPos.y; 0 <= row && row < mat.rows; row += stepDir.y)
						{
							if (whatPixelShouldMatch(row, column))
							{
								return { column, row };
							}
						}
					}
				}

				return { -1, -1 };
			}

			static inline constexpr Point FindCorner(Corner corner, Point offset, const cv::Mat& mat, PixelMatchCondition whatPixelShouldMatch, ScanDirection scanDirection = ScanDirection::Horizontal)
			{
				constexpr u8 verticalMask = 0b01;
				constexpr u8 horizontalMask = 0b10;
				// start pos
				// if top -> y = 0
				// if bottom -> y = mat.rows
				// if left -> x = 0
				// if right -> x = mat.cols
				Point startPos{ (mat.cols - 1) * ((corner & horizontalMask) != 0), (mat.rows - 1) * ((corner & verticalMask) != 0) };

				startPos.x += offset.x;
				startPos.y += offset.y;

				// scan direction
				// if top -> y++
				// if bottom -> y--
				// if left -> x++
				// if right -> x--
				Point stepDir{ 1 + -2 * ((corner & horizontalMask) != 0), 1 + -2 * ((corner & verticalMask) != 0) };

				for (int row = startPos.y; 0 <= row && row < mat.rows; row += stepDir.y)
				{
					for (int column = startPos.x; 0 <= column && column < mat.cols; column += stepDir.x)
					{
						if (whatPixelShouldMatch(row, column))
						{
							return { column, row };
						}
					}
				}

				return { -1, -1 };
			}

			static inline Rectangle TraceEdgeFromCorner(Point corner, Direction direction, const cv::Mat& mat, PixelMatchCondition whatPixelShouldMatch)
			{
				int xPos = corner.x;
				int yPos = corner.y;
				do
				{
					if (!whatPixelShouldMatch(yPos, xPos))
					{
						break;
					}
					xPos += direction.StepX();
					yPos += direction.StepY();
				} while (static_cast<u32>(xPos) < static_cast<u32>(mat.cols)
						 && static_cast<u32>(yPos) < static_cast<u32>(mat.rows));

				if (xPos == corner.x && yPos == corner.y)
				{
					return { -1, -1, -1, -1 };
				}

				int width = direction.IsHorizontal() ? std::abs(xPos - corner.x) : 1;
				int height = direction.IsVertical() ? std::abs(yPos - corner.y) : 1;
				return { std::min(corner.x, xPos), std::min(corner.y, yPos), width, height };
			}

			static inline int TraceEdgeLengthFromCorner(Point corner, Direction direction, const cv::Mat& mat, PixelMatchCondition whatPixelShouldMatch)
			{
				int xPos = corner.x;
				int yPos = corner.y;
				int length = 0;

				while (static_cast<u32>(xPos) < static_cast<u32>(mat.cols) &&
					   static_cast<u32>(yPos) < static_cast<u32>(mat.rows) &&
					   whatPixelShouldMatch(yPos, xPos))
				{
					++length;
					xPos += direction.StepX();
					yPos += direction.StepY();
				}

				return length;
			}

			static inline constexpr Point FindTopLeftCorner(const cv::Mat& mat, PixelMatchCondition whatPixelShouldMatch)
			{
				return FindCorner(Corner::TopLeft, mat, whatPixelShouldMatch);
			}

			static inline constexpr Point FindTopRightCorner(const cv::Mat& mat, PixelMatchCondition whatPixelShouldMatch)
			{
				return FindCorner(Corner::TopRight, mat, whatPixelShouldMatch);
			}

			static inline constexpr Point FindBottomLeftCorner(const cv::Mat& mat, PixelMatchCondition whatPixelShouldMatch)
			{
				return FindCorner(Corner::BottomLeft, mat, whatPixelShouldMatch);
			}

			static inline constexpr Point FindBottomRightCorner(const cv::Mat& mat, PixelMatchCondition whatPixelShouldMatch)
			{
				return FindCorner(Corner::BottomRight, mat, whatPixelShouldMatch);
			}

			static std::optional<Rectangle> FindBoundingBox(const cv::Mat& mat, const std::function<bool(int, int)>& whatPixelShouldMatch)
			{
				int left = INT_MAX, top = INT_MAX, right = 0, bottom = 0;
				bool foundOnePixel = false;
				for (int row = 0; row < mat.rows; row++)
				{
					int minCol = INT_MAX, maxCol = 0;
					bool rowContainsMatchingPixel = false;
					for (int col = 0; col < mat.cols; col++)
					{
						if (whatPixelShouldMatch(row, col))
						{
							rowContainsMatchingPixel = true;
							minCol = std::min(minCol, col);
							maxCol = std::max(maxCol, col);
						}
					}

					if (rowContainsMatchingPixel)
					{
						foundOnePixel = true;
						left = std::min(left, minCol);
						right = std::max(right, maxCol);
						top = std::min(top, row);
						bottom = std::max(bottom, row);
					}

				}
				if (!foundOnePixel)
				{
					return std::nullopt;
				}
				return Rectangle{ left, top, right - left + 1, bottom - top + 1 };
			}
		}
    }
}