#include "pch.h"
#include "base/documents_v2/doc_extractor.h"

namespace paplease {
	namespace documents {
		namespace v2 {

			namespace utils {

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


				typedef std::optional<cv::Mat>(*DocumentExtractionHandlerFunction)(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance);

				static std::optional<cv::Mat> IsExactSizeMatched(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance)
				{
					if (boundingBox.width == appearance.GetWidth() && boundingBox.height == appearance.GetHeight())
					{
						cv::Mat outMat = inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height));
						return outMat;
					}
					return std::nullopt;
				}

				static inline std::optional<cv::Mat> IsCornerMatched(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance)
				{
					cv::Mat canvas{ cv::Size(appearance.GetWidth(), appearance.GetHeight()), inspection.type(), CV_RGB(255, 255, 255) };
					cv::Mat cutDoc = inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height));

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

					return std::nullopt;
				}

				static inline std::optional<cv::Mat> IsEdgeMatched(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance)
				{
					cv::Mat canvas{ cv::Size(appearance.GetWidth(), appearance.GetHeight()), inspection.type(), CV_RGB(255, 255, 255) };
					cv::Mat cutDoc = inspection(cv::Rect(boundingBox.x, boundingBox.y, boundingBox.width, boundingBox.height));

					if (boundingBox.width == appearance.GetWidth())
					{ // Correct Width
						int alt = appearance.GetHeight() - cutDoc.rows;
						// bool hasTopEdge = false;
						bool hasTopEdge = (alt + cutDoc.cols) > canvas.rows;
						// bool hasTopEdge = IsCorner(inspection, boundingBox, true, true, appearance) || IsCorner(inspection, boundingBox, true, false, appearance);

						int top = (hasTopEdge) ? 0 : alt;

						cv::Mat insetImage(canvas, cv::Rect(0, top, cutDoc.cols, cutDoc.rows));
						cutDoc.copyTo(insetImage);

						return canvas;
					}

					if (boundingBox.height == appearance.GetHeight())
					{ // Correct Height
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

					return std::nullopt;
				}

			}  // namespace utils

			std::optional<cv::Mat> ExtractDocument(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance)
			{
				constexpr utils::DocumentExtractionHandlerFunction handlers[]
				{
					utils::IsExactSizeMatched,
		#if !STRICT_DOCUMENT_SCANNING
					Utils::IsCornerMatched
		#endif
				};

				assert(appearance.GetType() != AppearanceType::Invalid);

				cv::Mat mat; // if handler returns true, then mat is initialized

				for (auto& handler : handlers)
				{
					auto document = handler(inspection, boundingBox, appearance);
					if (document)
					{
						return document.value();
					}
				}

				return std::nullopt;
			}

		}  // namespace v2
	}  // namespace documents
}  // namespace paplease
