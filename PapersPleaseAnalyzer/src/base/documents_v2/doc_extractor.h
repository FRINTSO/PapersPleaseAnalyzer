#pragma once
#include <optional>

#include <opencv2/opencv.hpp>

#include "base/shape.h"
#include "base/documents_v2/doc_appearance.h"

namespace Documents::V2 {

	std::optional<cv::Mat> ExtractDocument(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance);

#if 0
	namespace Imported {
		static PassportType FindPassportTypeAndBoundingBox(const cv::Mat& inspection, Rectangle& outBoundingBox)
		{
			PassportType type = PassportType::Invalid;

			int left = INT_MAX;
			int top = INT_MAX;
			int right = 0;
			int bottom = 0;
			bool has_found_top_left = false;
			for (int y = 0; y < inspection.rows; y++)
			{
				const BgrColor* bgr = inspection.ptr<BgrColor>(y);

				int min_x = INT_MAX;
				int max_x = 0;
				for (int x = 0; x < inspection.cols; x++)
				{
					switch (BGR_VAL(bgr[x]))
					{
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
#if CHEEKY_OPTIMIZATION
					// cheeky optimization
					if (!has_found_top_left)
					{
						has_found_top_left = !has_found_top_left;

						int width = DocAppearance::Get((AppearanceType)type).GetWidth();
						int height = DocAppearance::Get((AppearanceType)type).GetHeight();

						if (y + height - 1 < inspection.rows && x + width - 1 < inspection.cols)
						{
							// bottom_right
							const BgrColor* bgr_2 = inspection.ptr<BgrColor>(y + height - 1);
							if (BGR_VAL(bgr_2[x + width - 1]) == RGB_VAL(DocAppearance::Get((AppearanceType)type).GetColors()[0]))
							{
								outBoundingBox = Rectangle{ x, y, width, height };
								return type;
							}
						}
					}
#endif
					if (x < min_x) min_x = x;
					if (x > max_x) max_x = x;
					if (y < top) top = y;
					if (y > bottom) bottom = y;
				}
				if (max_x)
				{
					if (min_x < left) left = min_x;
					if (max_x > right) right = max_x;
				}
			}

			outBoundingBox = Rectangle{ left, top, right - left + 1, bottom - top + 1 };

			return type;
		}

		static std::unordered_set<AppearanceType> FindAllDocumentsAppearanceType(const cv::Mat& inspection)
		{ // can return documents that aren't there..
			std::unordered_set<AppearanceType> types{};

			std::array<DocAppearance, 16> appearanceTypes{};
			for (size_t i = 0; i < appearanceTypes.size(); i++)
			{
				appearanceTypes[i] = DocAppearance::Get(static_cast<AppearanceType>(i + 1));
			}

			for (int y = 0; y < inspection.rows; y++)
			{
#if 0 // COLOR_OPTIMIZATION
				const BgrColor* bgr = inspection.ptr<BgrColor>(y);
				for (int x = 0; x < inspection.cols; x++)
				{
					for (const auto& docAppearance : appearanceTypes)
					{
						AppearanceType type = docAppearance.GetType();
						auto* colors = docAppearance.GetColors();
						auto colorCount = docAppearance.GetColorCount();

						for (size_t i = 0; i < colorCount; i++)
						{
							if (BGR_VAL(bgr[x]) == RGB_VAL(colors[i]) && !types.contains(type))
#else
				const BgrColor* bgr = inspection.ptr<BgrColor>(y);
				for (int x = 0; x < inspection.cols; x++)
				{
					for (const auto& docAppearance : appearanceTypes)
					{
						AppearanceType type = docAppearance.GetType();
						auto* colors = docAppearance.GetColors();
						auto colorCount = docAppearance.GetColorCount();

						for (size_t i = 0; i < colorCount; i++)
						{
							if (BGR_VAL(bgr[x]) == RGB_VAL(colors[i]) && !types.contains(type))
#endif
							{
								types.emplace(type);
								goto DO_LOOP;
							}
						}
					}
				DO_LOOP:;
				}
						}
			return types;
					}

		static bool IsBorderColor(const cv::Mat & inspection, int x, int y, const DocAppearance & appearance)
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

		static bool IsCorner(const cv::Mat & inspection, const Rectangle & boundingBox, bool isTop, bool isLeft, const DocAppearance & appearance)
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

		[[deprecated]]
		static inline cv::Mat CutoutDocumentProper(const cv::Mat & inspection, const Rectangle & boundingBox, const DocAppearance & appearance)
		{
			// Corner defintions
			// Top Left
			// A point that has color c and point to the right and below also has color c, while up and to the left does not have color c

			// If found bottom or top place image at bottom of white at x position (bottom - height)

			cv::Mat canvas(cv::Size(appearance.GetWidth(), appearance.GetHeight()), inspection.type(), CV_RGB(255, 255, 255));
			//cv::Mat canvas = cv::Mat::zeros(cv::Size(PASSPORT_WIDTH, PASSPORT_HEIGHT), inspection.type());
			if (appearance.GetType() == AppearanceType::Invalid)
			{
				std::cout << "INVALID\n";
				return canvas;
			}

			if (boundingBox.width == appearance.GetWidth() && boundingBox.height == appearance.GetHeight())
			{
				return inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height));
			}

			cv::Mat cutDoc = inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height));

			/* Hard to make a proper implementation
			// RE IMPLEMENT CODE, THAT ONCE CAUSED ISSUES, MAY CAUSE ISSUES AGAIN
			if (boundingBox.width == appearance.GetWidth()) { // Correct Width
				int alt = appearance.GetHeight() - cutDoc.rows;
				// bool hasTopEdge = false;
				bool hasTopEdge = (alt + cutDoc.cols) > canvas.rows;
				// bool hasTopEdge = IsCorner(inspection, boundingBox, true, true, appearance) || IsCorner(inspection, boundingBox, true, false, appearance);

				int top = (hasTopEdge) ? 0 : alt;

			cv::Mat insetImage(canvas, cv::Rect(0, top, cutDoc.cols, cutDoc.rows));
				cutDoc.copyTo(insetImage);

			return canvas;
			}

			if (boundingBox.height == appearance.GetHeight()) { // Correct Height
				// IsCorner(canvas)
				int alt = appearance.GetHeight() - cutDoc.cols;
				// bool hasLeftEdge = true;
				bool hasLeftEdge = (alt + cutDoc.cols) > canvas.cols;
				// bool hasLeftEdge = IsCorner(inspection, boundingBox, true, true, appearance) || IsCorner(inspection, boundingBox, false, true, appearance);
				int left = (hasLeftEdge) ? 0 : alt;

				cv::Mat insetImage(canvas, cv::Rect(left, 0, cutDoc.cols, cutDoc.rows));
				cutDoc.copyTo(insetImage);

				return canvas;
			}
			*/

			// If we can't see two corners try to find one

			if (IsCorner(inspection, boundingBox, true, true, appearance))
			{ // top left
				cv::Mat insetImage(canvas, cv::Rect(0, 0, cutDoc.cols, cutDoc.rows));
				cutDoc.copyTo(insetImage);

				return canvas;
			}
			else if (IsCorner(inspection, boundingBox, true, false, appearance))
			{ // top right
				cv::Mat insetImage(canvas, cv::Rect(appearance.GetWidth() - cutDoc.cols, 0, cutDoc.cols, cutDoc.rows));
				cutDoc.copyTo(insetImage);

				return canvas;
			}
			else if (IsCorner(inspection, boundingBox, false, true, appearance))
			{ // bottom left
				cv::Mat insetImage(canvas, cv::Rect(0, appearance.GetHeight() - cutDoc.rows, cutDoc.cols, cutDoc.rows));
				cutDoc.copyTo(insetImage);

				return canvas;
			}
			else if (IsCorner(inspection, boundingBox, false, false, appearance))
			{ // bottom right
				cv::Mat insetImage(canvas, cv::Rect(appearance.GetWidth() - cutDoc.cols, appearance.GetHeight() - cutDoc.rows, cutDoc.cols, cutDoc.rows));
				cutDoc.copyTo(insetImage);

				return canvas;
			}

			// return canvas;
			return {}; // empty
		}

	}
#endif

}