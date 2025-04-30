#pragma once
#include <algorithm>
#include <climits>
#include <functional>
#include <utility>

#include <opencv2/opencv.hpp>

#include "paplease/common/color.h"
#include "paplease/common/common.h"
#include "paplease/documents/doc_appearance.h"
#include "paplease/game_view.h"
#include "paplease/common/shape.h"


namespace paplease {
	namespace documents {

		static inline Rectangle FindDocumentBoundingBoxV2(const cv::Mat& view, ViewArea viewArea, const DocAppearance& appearance)
		{
			throw;
		}

		static inline Rectangle FindDocumentBoundingBox(const cv::Mat& inspection, const DocAppearance& appearance)
		{
			int left = INT_MAX;
			int top = INT_MAX;
			int right = 0;
			int bottom = 0;
			bool hasFoundTopLeft = false;

#if EXPERIMENTAL_MATCH_ONE_COLOR
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
#if EXPERIMENTAL_MATCH_ONE_COLOR
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
#if OPTIMIZE_CHEEKY
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
#if EXPERIMENTAL_MATCH_ONE_COLOR
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
#if OPTIMIZE_EFFECTIVE_SCANNING
						int maxHeight = top + appearance.GetHeight();
						maxScanY = (maxHeight > inspection.rows) ? inspection.rows : maxHeight;
#endif
					}

					bottom = std::max(bottom, y);

					if (min_x < left)
					{
						left = min_x;
#if OPTIMIZE_EFFECTIVE_SCANNING
						int maxWidth = left + appearance.GetWidth();
						maxScanX = (maxWidth > inspection.cols) ? inspection.cols : maxWidth;
#endif
					}
					if (max_x > right)
					{
						right = max_x;
#if OPTIMIZE_EFFECTIVE_SCANNING
						int minWidth = right - appearance.GetWidth() + 1;
						minScanX = (minWidth < 0) ? 0 : minWidth;
#endif
					}
				}
			}

			// Nothing was found
			if (left == INT_MAX && top == INT_MAX && !right && !bottom) return Rectangle{};

			return Rectangle{ left, top, right - left + 1, bottom - top + 1 };
		}

		namespace detail {

			static bool IsBorderColor(const cv::Mat& inspection, int x, int y, const DocAppearance& appearance)
			{
				const BgrColor* bgr = inspection.ptr<BgrColor>(y);

				bool isBorderColor = false;
				for (size_t i = 0; i < appearance.GetColorCount(); i++)
				{
					isBorderColor = BGR_VAL(bgr[x]) == RGB_VAL(appearance.GetColors()[i]);

					if (isBorderColor)
					{
						return isBorderColor;
					}
				}
				return false;

				switch (BGR_VAL(bgr[x]))
				{
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

			static bool IsCorner(const cv::Mat& inspection, const Rectangle& boundingBox, bool isTop, bool isLeft, const DocAppearance& appearance)
			{
				// if isTop is true, then we check down, else check up
				// if isLeft is true, then we check right, else check left

				int xCorner = boundingBox.x + (boundingBox.width - 1) * !isLeft;
				int yCorner = boundingBox.y + (boundingBox.height - 1) * !isTop;
				int width = boundingBox.width;
				int height = boundingBox.height;

				// Start by stepping closest to the inner corner
				int step = 0;
				do
				{
					if (step >= height || step >= width)
					{
						break;
					}
					// (-1 + 2 * bool) yields values -1 or 1
					if (!IsBorderColor(inspection, xCorner + step * (-1 + 2 * isLeft), yCorner + step * (-1 + 2 * isTop), appearance))
					{
						break;
					}
					step++;
				} while (true);

				// step will be +1, meaning if step is 0 then initial corner pixel did not have one of the border colors
				if (step == 0)
				{
					return false;
				}

				// step in x direction and y direction
				return IsBorderColor(inspection, xCorner + step * (-1 + 2 * isLeft), yCorner, appearance)
					&& IsBorderColor(inspection, xCorner, yCorner + step * (-1 + 2 * isTop), appearance);
			}


			typedef bool(*DocumentBoundingBoxValidatorFunction)(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance);

			static bool IsExactSizeMatched(const cv::Mat&, const Rectangle& boundingBox, const DocAppearance& appearance)
			{
				if (boundingBox.width == appearance.GetWidth() && boundingBox.height == appearance.GetHeight())
				{
					return true;
				}
				return false;
			}

			static inline bool IsCornerMatched(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance)
			{
				if (IsCorner(inspection, boundingBox, true, true, appearance))
				{ // top left
					return true;
				}
				else if (IsCorner(inspection, boundingBox, true, false, appearance))
				{ // top right
					return true;
				}
				else if (IsCorner(inspection, boundingBox, false, true, appearance))
				{ // bottom left
					return true;
				}
				else if (IsCorner(inspection, boundingBox, false, false, appearance))
				{ // bottom right
					return true;
				}

				return false;
			}

			static inline bool IsEdgeMatched(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance)
			{
				if (boundingBox.width == appearance.GetWidth())
				{ // Correct Width
					return true;
				}

				if (boundingBox.height == appearance.GetHeight())
				{ // Correct Height
					return true;
				}

				return false;
			}

			static inline bool IsTooLarge(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance)
			{
				if (boundingBox.width > appearance.GetWidth())
				{
					return true;
				}

				if (boundingBox.height > appearance.GetHeight())
				{
					return true;
				}

				return false;
			}

		}  // namespace detail

		static inline bool ValidateDocumentBoundingBox(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance)
		{
			constexpr detail::DocumentBoundingBoxValidatorFunction negativeValidators[]{
				detail::IsTooLarge,
			};

			for (auto& negativeValidator : negativeValidators)
			{
				auto shouldBeDiscarded = negativeValidator(inspection, boundingBox, appearance);
				if (shouldBeDiscarded)
				{
					return false;
				}
			}

			constexpr detail::DocumentBoundingBoxValidatorFunction validators[]
			{
				detail::IsExactSizeMatched,
	#if !ENABLE_STRICT_DOCUMENT_SCANNING
				detail::IsCornerMatched
	#endif
			};

			assert(appearance.GetType() != AppearanceType::Invalid);

			for (auto& validator : validators)
			{
				auto isValid = validator(inspection, boundingBox, appearance);
				if (isValid)
				{
					return true;
				}
			}

			return false;
		}
		
		static inline bool ValidateDocumentBoundingBoxInBoothView(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance)
		{
			constexpr detail::DocumentBoundingBoxValidatorFunction negativeValidators[]{
				detail::IsTooLarge,
			};

			for (auto& negativeValidator : negativeValidators)
			{
				auto shouldBeDiscarded = negativeValidator(inspection, boundingBox, appearance);
				if (shouldBeDiscarded)
				{
					return false;
				}
			}

			constexpr detail::DocumentBoundingBoxValidatorFunction validators[]
			{
				detail::IsExactSizeMatched,
	#if !ENABLE_STRICT_DOCUMENT_SCANNING
				detail::IsCornerMatched
	#endif
			};

			assert(appearance.GetType() != AppearanceType::Invalid);

			for (auto& validator : validators)
			{
				auto isValid = validator(inspection, boundingBox, appearance);
				if (isValid)
				{
					return true;
				}
			}

			return false;
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
