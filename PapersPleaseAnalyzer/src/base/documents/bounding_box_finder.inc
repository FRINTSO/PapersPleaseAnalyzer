#pragma once
#include <array>

#include <opencv2/opencv.hpp>

#include "base/color.h"
#include "base/shape.h"

inline Rectangle FindDocumentBoundingBox(const cv::Mat& inspection, const RgbColor* colors, const size_t size) {
	int left = INT_MAX;
	int top = INT_MAX;
	int right = 0;
	int bottom = 0;

	for (int y = 0; y < inspection.rows; y++) {
		const RgbColor* bgr = inspection.ptr<RgbColor>(y);

		int min_x = INT_MAX;
		int max_x = 0;
		for (int x = 0; x < inspection.cols; x++) {
			// CHECK IF BGR_VAL(bgr[x]) is in colors
			bool colorMatch = false;
			for (size_t i = 0; i < size; i++) {
				if (RGB_VAL(colors[i]) == BGR_VAL(bgr[x])) {
					colorMatch = true;
					break;
				}
			}
			if (!colorMatch) continue;

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

	// Nothing was found
	if (left == INT_MAX && top == INT_MAX && !right && !bottom) return Rectangle{};

	return Rectangle{ left, top, right - left + 1, bottom - top + 1 };
}