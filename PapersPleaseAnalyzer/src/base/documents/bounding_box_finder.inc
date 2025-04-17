#pragma once
#include <algorithm>
#include <climits>
#include <functional>
#include <utility>

#include <opencv2/opencv.hpp>

#include "base/color.h"
#include "base/common.h"
#include "base/documents/doc_appearance.h"
#include "base/game_view.h"
#include "base/shape.h"


namespace paplease {
	namespace documents {

		static inline Rectangle FindDocumentBoundingBox(const GameView& gameView, const DocAppearance& appearance)
		{
			int left = INT_MAX;
			int top = INT_MAX;
			int right = 0;
			int bottom = 0;
			bool hasFoundTopLeft = false;

			const auto& inspection = gameView.inspection;
#if MATCH_ONE_COLOR
			const auto color = RGB_VAL(appearance.GetColors()[0]);
#else
			const RgbColor* colors = appearance.GetColors();
			const size_t size = appearance.GetColorCount();
#endif

			int maxScanY = inspection.rows;
			int minScanX = 0;
			int maxScanX = inspection.cols;

			for (int y = 0; y < maxScanY; y++)
			{
				const BgrColor* bgr = inspection.ptr<BgrColor>(y);

				int min_x = INT_MAX;
				int max_x = 0;
				bool colorMatchedInRow = false;

				for (int x = minScanX; x < maxScanX; x++)
				{
					// CHECK IF BGR_VAL(bgr[x]) is in colors
#if MATCH_ONE_COLOR
					if (color != BGR_VAL(bgr[x])) continue;
#else
					bool colorMatch = false;
					for (size_t i = 0; i < size; i++)
					{
						if (RGB_VAL(colors[i]) == BGR_VAL(bgr[x]))
						{
							colorMatch = true;
							break;
						}
					}
					if (!colorMatch) continue;
#endif
#if CHEEKY_OPTIMIZATION
					// cheeky optimization
					if (!hasFoundTopLeft)
					{
						hasFoundTopLeft = !hasFoundTopLeft;

						int width = appearance.GetWidth();
						int height = appearance.GetHeight();

						if (y + height - 1 < inspection.rows && x + width - 1 < inspection.cols)
						{
							// bottom_right
							const auto& bottomRightColor = *inspection.ptr<BgrColor>(y + height - 1, x + width - 1);
#if MATCH_ONE_COLOR
							if (color == BGR_VAL(bgr[x]))
								return Rectangle{ x,y,width,height };
#else
							for (size_t i = 0; i < size; i++)
							{
								if (RGB_VAL(colors[i]) == BGR_VAL(bottomRightColor))
								{
									return Rectangle{ x, y, width, height };
								}
							}
#endif
						}
					}
#endif

					colorMatchedInRow = true;

					min_x = std::min(min_x, x);
					max_x = std::max(max_x, x);
				}

				if (colorMatchedInRow)
				{
					if (y < top)
					{
						top = y;
#if EFFECTIVE_SCANNING_OPTIMIZATION
						int maxHeight = top + appearance.GetHeight();
						maxScanY = (maxHeight > inspection.rows) ? inspection.rows : maxHeight;
#endif
					}

					bottom = std::max(bottom, y);

					if (min_x < left)
					{
						left = min_x;
#if EFFECTIVE_SCANNING_OPTIMIZATION
						int maxWidth = left + appearance.GetWidth();
						maxScanX = (maxWidth > inspection.cols) ? inspection.cols : maxWidth;
#endif
					}
					if (max_x > right)
					{
						right = max_x;
#if EFFECTIVE_SCANNING_OPTIMIZATION
						int minWidth = right - appearance.GetWidth() + 1;
						minScanX = (minWidth < 0) ? 0 : minWidth;
#endif
					}
				}
			}

			// Nothing was found
			if (left == INT_MAX && top == INT_MAX && !right && !bottom) return Rectangle{};

			//cv::imshow("DBG_DOC_BOX", inspection(cv::Rect(left, top, right - left + 1, bottom - top + 1)));
			//cv::waitKey();

			return Rectangle{ left, top, right - left + 1, bottom - top + 1 };
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

	}  // namespace documents
}  // namespace paplease
