#include "pch.h"
#include "paplease/scannable/doc_scan.h"

#include "paplease/documents/bounding_box_finder.inc"
#include "paplease/utils/enum_range.h"

namespace paplease {
	namespace scannable {
		using namespace documents;

#pragma region OldFuncs
#pragma region Static Functions
		static consteval std::array<RgbColor, 7> GetPassportColors()
		{
			return std::array<RgbColor, 7>{
				DocAppearance::GetInstant(AppearanceType::Passport_Antegria).GetColors()[0],
					DocAppearance::GetInstant(AppearanceType::Passport_Arstotzka).GetColors()[0],
					DocAppearance::GetInstant(AppearanceType::Passport_Impor).GetColors()[0],
					DocAppearance::GetInstant(AppearanceType::Passport_Kolechia).GetColors()[0],
					DocAppearance::GetInstant(AppearanceType::Passport_Obristan).GetColors()[0],
					DocAppearance::GetInstant(AppearanceType::Passport_Republia).GetColors()[0],
					DocAppearance::GetInstant(AppearanceType::Passport_UnitedFederation).GetColors()[0],
			};
		}

		static PassportType FindPassportType(const cv::Mat& mat)
		{
			static constexpr auto passportTypesColor = GetPassportColors();

			for (int row = 0; row < mat.rows; row++)
			{
				for (int col = 0; col < mat.cols; col++)
				{
					for (int i = 0; i < passportTypesColor.size(); i++)
					{
#if OPTIMIZE_COLOR
						if (RgbVal(passportTypesColor[i]) == BgrVal(*mat.ptr<BgrColor>(row, col)))
						{
							return static_cast<PassportType>(static_cast<int>(PassportType::Antegria) + i);
						}
#else
						if (RGB_VAL(passportTypesColor[i]) == BGR_VAL(*mat.ptr<BgrColor>(row, col)))
						{
							return static_cast<PassportType>(static_cast<int>(PassportType::Antegria) + i);
						}
#endif
					}
				}
			}
			return PassportType::Invalid;
		}


#pragma endregion

#pragma region Document Extraction

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

		}  // namespace detail


		std::optional<cv::Mat> ExtractDocument(const cv::Mat& inspection, const Rectangle& boundingBox, const DocAppearance& appearance)
		{
			constexpr detail::DocumentExtractionHandlerFunction handlers[]
			{
				detail::IsExactSizeMatched,
	#if !ENABLE_STRICT_DOCUMENT_SCANNING
				detail::IsCornerMatched
	#endif
			};

			assert(appearance.GetType() != AppearanceType::Invalid);

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

#pragma endregion

#if OLD_CODE
		core::FixedArray<Doc, MaxDocumentsScanned> DocScanResult::ExtractDocs() const
		{
			core::FixedArray<Doc, MaxDocumentsScanned> docs{};
			for (auto& [boundingBox, appearanceType, isBooth] : scannedDocuments)
			{
				const DocAppearance& docAppearance = DocAppearance::GetRef(appearanceType);
				auto docOpt = ExtractDocument(this->sourceImage, boundingBox, docAppearance);
				if (!docOpt.has_value())
				{
					LOG_ERR("Could not extract document!");
					continue;
				}
				//assert(docOpt.has_value() && );

				auto documentType = ToDocType(appearanceType);
				auto passportType = ToPassportType(appearanceType);

				//docs.EmplaceBack(Doc{ std::move(docOpt.value()), documentType, passportType });
			}

			return docs;
		}


		static inline auto GetAppearanceTypes()
		{
			using namespace documents;
			std::array<const DocAppearance*, static_cast<size_t>(AppearanceType::Count)> appearanceTypes{};

			for (auto appearanceType : utils::enum_range(AppearanceType::AccessPermit, AppearanceType::Passport_UnitedFederation))
			{
				appearanceTypes[static_cast<size_t>(appearanceType)] = &DocAppearance::GetRef(appearanceType);
			}

			return appearanceTypes;
		}

		DocScanResult ScanForDocuments(const cv::Mat& binSearchArea, ViewArea viewArea)
		{
			static const auto appearanceTypes = GetAppearanceTypes();

			ScannedDocList scannedDocs{};

			/*for (auto docAppearance : appearanceTypes)
			{
				auto boundingBox = FindDocumentBoundingBox(binSearchArea, *docAppearance);
				if (boundingBox.Empty()) continue;

				scannedDocs.EmplaceBack(
					DocView{ boundingBox, docAppearance->GetType(), viewArea }
				);
			}*/

			return DocScanResult{ binSearchArea, scannedDocs };
		}

		std::optional<DocView> FindDocument(const GameView& gameView, const DocType documentType)
		{
			if (documentType == DocType::Invalid) return std::nullopt;

			PassportType passportType = PassportType::Invalid;

			if (documentType == DocType::Passport)
			{
				passportType = FindPassportType(gameView);
			}

			auto appearanceType = ToAppearanceType(documentType, passportType);
			auto appearance = DocAppearance::Get(appearanceType);

			auto boundingBox = FindDocumentBoundingBox(gameView.inspection(), appearance);
			if (boundingBox.Empty()) return std::nullopt;

			return DocView{ boundingBox, appearanceType, ViewArea::Invalid };
		}
#endif

#pragma endregion

#pragma region Consteval Magic
		namespace detail {

			static consteval bool HasColor(int* storedColors, int colorsCounted, int color)
			{
				for (size_t i = 0; i < colorsCounted; i++)
				{
					if (storedColors[i] == color)
					{
						return true;
					}
				}
				return false;
			}

			static consteval size_t CountUniqueColors()
			{
				int storedColors[50]{};
				int colorsCounted = 0;

				for (auto appearanceType : utils::enum_range(AppearanceType::AccessPermit, AppearanceType::Passport_UnitedFederation))
				{
					for (size_t i = 0; i < DocAppearance::GetInstant(appearanceType).GetColorCount(); i++)
					{
#if OPTIMIZE_COLOR
						const int currentColorCode = RgbVal(DocAppearance::GetInstant(appearanceType).GetColors()[i]);
#else
						const int currentColorCode = RGB_VAL(DocAppearance::GetInstant(appearanceType).GetColors()[i]);
#endif
						if (!HasColor(storedColors, colorsCounted, currentColorCode))
						{
							storedColors[colorsCounted++] = currentColorCode;
						}

					}
				}
				return colorsCounted;
			}

			template<typename T>
			struct HashInteger
			{
				static_assert(std::is_integral_v<T>, "HashInteger only supports integral types");
				constexpr auto operator()(T value)
				{
					using Unsigned = std::make_unsigned_t<T>;
					constexpr Unsigned knuth_constant = 2654435761u; // from Knuth's multiplicative method

					return static_cast<Unsigned>(value) * knuth_constant;
				}
			};

			static consteval auto GenerateColorAppearanceTable()
			{
				using ColorCode = int;
				constexpr float LoadFactor = 0.50f;
				constexpr size_t EstimatedSize = static_cast<size_t>(CountUniqueColors() / LoadFactor);

				core::FixedHashTable<ColorCode, AppearanceType, EstimatedSize, HashInteger<ColorCode>> table{};

				for (AppearanceType appearanceType : utils::enum_range(AppearanceType::AccessPermit, AppearanceType::Passport_UnitedFederation))
				{
					const auto appearance = DocAppearance::GetInstant(appearanceType);
					const auto* colors = appearance.GetColors();
					const size_t colorCount = appearance.GetColorCount();

					for (size_t i = 0; i < colorCount; i++)
					{
#if OPTIMIZE_COLOR
						const ColorCode code = RgbVal(colors[i]);
#else
						const ColorCode code = RGB_VAL(colors[i]);
#endif
						bool success = table.Set(code, appearanceType);
						assert(success && "Failed to insert into color-appearance table at compile time");
					}
				}
				return table;
			}
		}  // namespace detail
#pragma endregion

		static cv::Mat PrepareBoothForDocumentScanning(const cv::Mat& booth)
		{
			// Minimized documents
			static constinit HSVConfig config1{ 36, 36, 0 ,255, 0, 255 };
			auto binary1 = ToHSVBinary(booth, config1);
			static constinit HSVConfig config2{ 11, 24, 77 ,147, 66, 139 };
			auto binary2 = ToHSVBinary(booth, config2);
			static auto boothDocArea = LoadImageFile("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\booth\\booth_doc_area.png");
			cv::Mat binaryMask;
			cv::bitwise_or(binary1, binary2, binaryMask, boothDocArea);

			cv::Mat result;
			cv::bitwise_and(booth, booth, result, binaryMask ^ 255);

			cv::Mat result2;
			cv::bitwise_and(result, result, result2, boothDocArea);
			return result2;
		}

		static cv::Mat PrepareInspectionForDocumentScanning(const cv::Mat& inspection)
		{
			// NORMAL BOOTH, WORKS TO REMOVE BACKGROUND
			static HSLConfig config{ 0, 179, 45, 255, 0, 255 };
			auto mask = ToHSLBinary(inspection, config);
			cv::Mat result;
			cv::bitwise_and(inspection, inspection, result, mask);

			// HIGHLIGHT DISCREPANCIES
			static HSLConfig highlightDiscrepanciesActiveConfig{ 0 , 0 , 112, 112,  157, 157 };
			auto highlightMask = ToHSLBinary(inspection, highlightDiscrepanciesActiveConfig);

			static HSLConfig majorDocumentLocatorConfig{ 0, 179, 135, 255, 0, 255 };
			// cv::imshow("Test", ToHSLBinary(inspection, majorDocumentLocatorConfig));

			static HSVConfig passportFinderConfig{};
			// test::documents::find_hsv(inspection, passportFinderConfig);
			//cv::imshow("Filluo", ToHSVBinary(inspection, config1));

			//cv::imshow("Result", result);
			return result;
		}

		static inline cv::Mat GetProcessedView(const GameView& gameView, ViewArea viewArea)
		{
			return viewArea == ViewArea::BoothView
				? PrepareBoothForDocumentScanning(gameView.booth())
				: PrepareInspectionForDocumentScanning(gameView.inspection());
		}

		AppearanceCollection ScanForDocumentTypes(const GameView& gameView, ViewArea viewArea)
		{
			// the mat should be processed so that the background is black and the documents are their regular color
			// IDEA: Scan with subtracted scan space
			// Locate a document "extract it", and continue scanning but don't scan that part

			const auto& mat = GetProcessedView(gameView, viewArea);

			static auto table = detail::GenerateColorAppearanceTable();
			core::FixedHashSet<documents::AppearanceType, static_cast<size_t>(documents::AppearanceType::Count)> types{};

			for (size_t row = 0; row < mat.rows; row++)
			{
				const auto* color = mat.ptr<BgrColor>(row);
				for (size_t column = 0; column < mat.cols; column++)
				{
#if OPTIMIZE_COLOR
					int currentColor = BgrVal(color[column]);
#else
					int currentColor = BGR_VAL(color[column]);
#endif

					AppearanceType* type;
					if (table.Get(currentColor, type))
					{
						types.Insert(*type);
					}
				}
			}

			AppearanceCollection collection{};
			for (auto type : types)
			{
				collection.Add(type);
			}
			return collection;
		}

		std::optional<documents::DocView> ScanForDocument(const GameView& gameView, ViewArea viewArea, documents::DocType documentType)
		{
			const auto& mat = GetProcessedView(gameView, viewArea);

			if (documentType == DocType::Invalid) return std::nullopt;

			PassportType passportType = PassportType::Invalid;

			if (documentType == DocType::Passport)
			{
				passportType = FindPassportType(mat);
			}

			auto appearanceType = ToAppearanceType(documentType, passportType);
			const auto& appearance = DocAppearance::GetRef(appearanceType);

			const auto boundingBox = FindDocumentBoundingBox(mat, appearance);
			const bool success = ValidateDocumentBoundingBox(mat, boundingBox, appearance);

			if (boundingBox.Empty() || !success) return std::nullopt;
			return DocView{ boundingBox, appearanceType, viewArea };
		}
		DocViewCollection ScanForDocuments(const GameView& gameView, ViewArea viewArea)
		{
			const auto& mat = GetProcessedView(gameView, viewArea);
			DocViewCollection collection;

			// Cheap implementation
			for (auto appearanceType : utils::enum_range(AppearanceType::AccessPermit, AppearanceType::Passport_UnitedFederation))
			{
				const auto& docAppearance = DocAppearance::GetRef(appearanceType);
				const auto boundingBox = FindDocumentBoundingBox(mat, docAppearance);
				const bool success = viewArea == ViewArea::InspectionView
					? ValidateDocumentBoundingBox(mat, boundingBox, docAppearance)
					: ValidateDocumentBoundingBoxInBoothView(mat, boundingBox, docAppearance);  // TODO: Fix this to filter bounding boxes in booth view

				if (!boundingBox.Empty() && success)
				{
					collection.EmplaceBack(DocView{ boundingBox, appearanceType, viewArea });
				}
			}
			return collection;
		}

	}  // namespace scannable
}  // namespace paplease
