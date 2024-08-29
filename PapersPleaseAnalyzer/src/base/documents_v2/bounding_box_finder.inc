#pragma once
#include <algorithm>
#include <array>
#include <unordered_set>

#include <opencv2/opencv.hpp>

#include "base/color.h"
#include "base/common.h"
#include "base/documents_v2/doc_appearance.h"
#include "base/game_view.h"
#include "base/shape.h"


namespace Documents::V2
{

	inline Rectangle FindDocumentBoundingBox(const GameView& gameView, const DocAppearance& appearance) {
		int left = INT_MAX;
		int top = INT_MAX;
		int right = 0;
		int bottom = 0;
		bool hasFoundtopLeft = false;

		const auto& inspection = gameView.inspection;

		const RgbColor* colors = appearance.GetColors();
		const size_t size = appearance.GetColorCount();

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
				bool colorMatch = false;
				for (size_t i = 0; i < size; i++) {
					if (RGB_VAL(colors[i]) == BGR_VAL(bgr[x]))
					{
						std::cout << PRINT_RGB(bgr[x]) << "\n";
						std::cout << PRINT_RGB(colors[i]) << "\n\n";
						colorMatch = true;
						break;
					}
				}
				if (!colorMatch) continue;

#if CHEEKY_OPTIMIZATION
				// cheeky optimization
				if (!hasFoundtopLeft) {
					hasFoundtopLeft = !hasFoundtopLeft;

					int width = appearance.GetWidth();
					int height = appearance.GetHeight();

					if (y + height - 1 < inspection.rows && x + width - 1 < inspection.cols)
					{
						// bottom_right
						const auto& bottomRightColor = *inspection.ptr<BgrColor>(y + height - 1, x + width - 1);

						for (size_t i = 0; i < size; i++)
						{
							if (RGB_VAL(colors[i]) == BGR_VAL(bottomRightColor))
							{
								return Rectangle{ x, y, width, height };
							}
						}
					}
				}
#endif

				colorMatchedInRow = true;

				min_x = std::min(min_x, x);
				max_x = std::max(max_x, x);
			}

			if (colorMatchedInRow)
			{
				if (y < top) {
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

	inline Rectangle ShrinkDocumentBoundingBox(const cv::Mat& inspection, const Rectangle& boundingBox, const RgbColor* colors) {
		return {};
	}

}