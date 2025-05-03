#include "pch.h"
#include "paplease/scannable/booth.h"

#include "paplease/common/color.h"
#include "paplease/common/image_process.h"
#include "paplease/core/resource_manager.h"
#include "paplease/documents/doc_data.h"
#include "paplease/documents/doc_layout.h"
#include "test/documents/test_hsv.h"

#include <opencv2/opencv.hpp>

#include "paplease/documents/bounding_box_finder.inc"

namespace paplease {
	namespace scannable {

		using namespace documents;
		using namespace documents::data;

#pragma region Image Processing

		static cv::Mat PrepareBoothForOCR(const cv::Mat& booth)
		{
			static constinit HSVConfig config{ 12, 18, 0, 255, 106, 106 };
			auto mask = ToHSVBinary(booth, config);
			mask(ScaledRectangle(168, 416, 22, 12)) ^= 255;
			return mask;
		}

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

#pragma endregion

#pragma region Entrant Headshot Processing

		static inline cv::Mat LoadHeadshotMask()
		{
			return LoadImageFile("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\booth\\headshot_mask.png") ^ 255;
		}

#define CHAT_GPT 0
#if CHAT_GPT

		// Constants
		static constinit HSVConfig kHeadshotReadyConfig{ 10, 179, 0, 60, 53, 100 };
		static constinit HSVConfig kSilhouetteConfig{ 0, 23, 1, 71, 0, 255 };
		static const cv::Mat kHeadshotMask = LoadHeadshotMask();

		// Structs
		struct HeadTypeInfo
		{
			int value;
			HSVConfig config;
			const char* label;
		};

		static constinit HeadTypeInfo g_KnownHeadTypes[] = {
			{ rgb_color_v<67, 118, 0>, {1, 179, 0, 255, 0, 255}, "Type1" },
			{ rgb_color_v<87, 199, 11>, {1, 121, 0, 255, 0, 255}, "Type2" },
			{ rgb_color_v<91, 188, 8>, {0, 179, 126, 255, 0, 255}, "Type3" },
			{ rgb_color_v<98, 83, 105>, {66, 179, 0, 255, 0, 255}, "Type4" },
			{ rgb_color_v<80, 96, 128>, {37, 179, 0, 255, 0, 255}, "Type5" },
			{ rgb_color_v<80, 61, 69>, {1, 179, 0, 251, 0, 255}, "Type6" },
		};

		// Utility
		static inline std::optional<cv::Mat> PreprocessHeadshot(const cv::Mat& input)
		{
			if (cv::countNonZero(ToHSVBinary(input, kHeadshotReadyConfig)) != 0)
				return std::nullopt;

			cv::Mat binary = ToHSVBinary(input, kSilhouetteConfig) ^ kHeadshotMask;
			if (cv::countNonZero(binary ^ 255) == 0)
				return std::nullopt;

			return binary;
		}

		static constexpr std::optional<cv::Rect> FindSilhouetteBoundingBox(const cv::Mat& binary)
		{
			return documents::FindBoundingBox(binary, [&](int r, int c)
			{
				return binary.at<uchar>(r, c) == 0;
			});
		}

		static inline std::optional<cv::Mat> ExtractMaskedHSV(const cv::Mat& colorImage, const cv::Mat& binaryMask, const cv::Rect& roi)
		{
			cv::Mat originalROI(colorImage, roi), maskROI(binaryMask, roi);
			cv::Mat hsv;
			cv::cvtColor(originalROI, hsv, cv::COLOR_BGR2HSV);
			cv::Mat maskedHSV;
			cv::bitwise_and(hsv, hsv, maskedHSV, maskROI ^ 255);
			return maskedHSV;
		}

		static constexpr std::optional<HeadTypeInfo*> DetectHeadType(const cv::Mat& hsvImage)
		{
			for (int row = 0; row < hsvImage.rows; ++row)
			{
				for (int col = 0; col < hsvImage.cols; ++col)
				{
					int bgrVal = BGR_VAL(*hsvImage.ptr<BgrColor>(row, col));
					for (auto& type : g_KnownHeadTypes)
						if (bgrVal == type.value)
							return &type;
				}
			}
			return std::nullopt;
		}

		// FUNCTIONS
		std::optional<cv::Mat> BoothData::ExtractSilhouette() const
		{
			if (!applicantHeadshot) return std::nullopt;
			auto binary = PreprocessHeadshot(applicantHeadshot->m_mat);
			if (!binary) return std::nullopt;

			auto box = FindSilhouetteBoundingBox(*binary);
			return box ? std::optional<cv::Mat>{cv::Mat(*binary, *box)} : std::nullopt;
		}

		std::optional<cv::Mat> BoothData::ExtractHead() const
		{
			if (!applicantHeadshot) return std::nullopt;
			auto binary = PreprocessHeadshot(applicantHeadshot->m_mat);
			if (!binary) return std::nullopt;

			auto box = FindSilhouetteBoundingBox(*binary);
			if (!box) return std::nullopt;

			auto maskedHSV = ExtractMaskedHSV(applicantHeadshot->m_mat, *binary, *box);
			if (!maskedHSV) return std::nullopt;

			auto typeOpt = DetectHeadType(*maskedHSV);
			if (!typeOpt) return std::nullopt;

			auto* type = *typeOpt;
			std::cout << type->label << '\n';

			return type->config.IsEmpty()
				? test::documents::find_hsv(*maskedHSV, type->config)
				: ToHSVBinary(*maskedHSV, type->config);
		}

		std::optional<cv::Mat> BoothData::ExtractFace() const
		{
			if (!applicantHeadshot) return std::nullopt;
			auto binary = PreprocessHeadshot(applicantHeadshot->m_mat);
			if (!binary) return std::nullopt;

			auto box = FindSilhouetteBoundingBox(*binary);
			if (!box) return std::nullopt;

			auto maskedHSV = ExtractMaskedHSV(applicantHeadshot->m_mat, *binary, *box);
			if (!maskedHSV) return std::nullopt;

			auto typeOpt = DetectHeadType(*maskedHSV);
			if (!typeOpt) return std::nullopt;
			auto* type = *typeOpt;

			cv::Mat faceMask = type->config.IsEmpty()
				? test::documents::find_hsv(*maskedHSV, type->config)
				: ToHSVBinary(*maskedHSV, type->config);

			auto head = this->ExtractHead();
			if (!head) return std::nullopt;

			return ((faceMask ^ 255) & *head) ^ 255;
		}

		std::optional<cv::Mat> BoothData::ExtractBinaryHeadshot() const
		{
			auto silhouette = ExtractSilhouette();
			auto head = ExtractHead();
			auto face = ExtractFace();
			if (!head || !face) return silhouette;

			cv::Mat merged;
			cv::bitwise_and(*face, *face, merged, *head);

			if (silhouette)
			{
				cv::Mat combined;
				cv::bitwise_or(*silhouette, merged, combined);
				return combined;
			}
			return merged;
		}

		documents::data::Photo BoothData::PhotoToBinaryHeadshotPhoto() const
		{
			return documents::data::Photo{ PreprocessHeadshot(applicantHeadshot ? applicantHeadshot->m_mat : cv::Mat{}).value_or(cv::Mat{}) };
		}

		static std::optional<SIUnitValue> FindApplicantHeight(const documents::data::Photo& photo)
		{
			auto binary = PreprocessHeadshot(photo.m_mat);
			if (!binary) return std::nullopt;

			auto box = FindSilhouetteBoundingBox(*binary);
			if (!box) return std::nullopt;

			constexpr int minHeightCm = 100;
			constexpr int cmPerLine = 10;
			constexpr scaled<int> lineSpacingPx = 22;

			int pixelHeight = box->height;
			int height = minHeightCm + (pixelHeight * cmPerLine / lineSpacingPx);
			return SIUnitValue(height, SIUnit::CM);
		}

#else

		static std::optional<cv::Mat> PreprocessApplicantHeadshot(const cv::Mat& applicantInBooth)
		{
			// White text
			//constexpr HSVConfig exactTextColorToWhite{ 117, 117, 45, 45, 63, 63};
			static constinit HSVConfig isHeadshotScanReadyConfig{ 10, 179, 0, 60, 53, 100 };
			bool isHeadshotReady = cv::countNonZero(ToHSVBinary(applicantInBooth, isHeadshotScanReadyConfig)) == 0;
			if (!isHeadshotReady)
			{
				return std::nullopt;
			}

			// Extract headshot black silhouette + height measurements + black text
			static constinit HSVConfig headshotHsvConfig{ 0, 23, 1, 71, 0, 255 };
			auto binaryHeadshot = ToHSVBinary(applicantInBooth, headshotHsvConfig);

			static const cv::Mat headshotMask = LoadHeadshotMask();

			binaryHeadshot ^= headshotMask;

			if (cv::countNonZero(binaryHeadshot ^ 255) == 0)
			{
				return std::nullopt;
			}
			return binaryHeadshot;
		}

		struct HeadTypeInfo
		{
			int value;
			HSVConfig config;
			const char* label;
		};

		static constinit HeadTypeInfo g_KnownHeadTypes[] = {
			{ rgb_color_v<67, 118, 0>, {1, 179, 0, 255, 0, 255}, "Type1" },
			{ rgb_color_v<87, 199, 11>, {1, 121, 0, 255, 0, 255}, "Type2"},
			{ rgb_color_v<91, 188, 8>, {0, 179, 126, 255, 0, 255}, "Type3" },
			{ rgb_color_v<98, 83, 105>, {66, 179, 0, 255, 0, 255}, "Type4" },
			{ rgb_color_v<80, 96, 128>, {37, 179, 0, 255, 0, 255}, "Type5" },
			{ rgb_color_v<80, 61, 69>, {1, 179, 0, 251, 0, 255}, "Type6" },
		};

		static std::optional<HeadTypeInfo*> DetectHeadType(const cv::Mat& maskedHSV)
		{
			for (size_t row = 0; row < maskedHSV.rows; row++)
			{
				for (size_t col = 0; col < maskedHSV.cols; col++)
				{
					const int bgr_val = BGR_VAL(*maskedHSV.ptr<BgrColor>(row, col));

					for (auto& type : g_KnownHeadTypes)
					{
						if (bgr_val == type.value)
							return &type;
					}
				}
			}
			return std::nullopt;
		}

		std::optional<cv::Mat> BoothData::ExtractSilhouette() const
		{
			if (!applicantHeadshot)
				return std::nullopt;

			auto preprocessed = PreprocessApplicantHeadshot(applicantHeadshot.value());
			if (!preprocessed)
				return std::nullopt;

			const cv::Mat& headshot = preprocessed.value();

			auto boundingBoxOpt = documents::FindBoundingBox(headshot, [&](int row, int col)
			{
				return headshot.at<uchar>(row, col) == 0;
			});

			if (!boundingBoxOpt)
				return std::nullopt;

			return cv::Mat(headshot, *boundingBoxOpt);
		}
		std::optional<cv::Mat> BoothData::ExtractHead() const
		{
			if (!applicantHeadshot)
				return std::nullopt;

			auto preprocessed = PreprocessApplicantHeadshot(applicantHeadshot.value());
			if (!preprocessed)
				return std::nullopt;

			const cv::Mat& headshot = preprocessed.value();

			auto boundingBoxOpt = documents::FindBoundingBox(headshot, [&](int row, int col)
			{
				return headshot.at<uchar>(row, col) == 0;
			});

			if (!boundingBoxOpt)
				return std::nullopt;

			cv::Mat originalCutout(applicantHeadshot.value(), *boundingBoxOpt);
			cv::Mat silhouetteMask(headshot, *boundingBoxOpt);

			cv::Mat hsvImage;
			cv::cvtColor(originalCutout, hsvImage, cv::COLOR_BGR2HSV);

			cv::Mat maskedHSV;
			cv::bitwise_and(hsvImage, hsvImage, maskedHSV, silhouetteMask ^ 255);

			HSVConfig* selectedConfig = nullptr;
			cv::Mat returnImage;

			auto typeOpt = DetectHeadType(maskedHSV);
			if (!typeOpt)
			{
				cv::imshow("Unknown Head Type", maskedHSV);
				cv::waitKey();
				return std::nullopt;

			}
			auto* type = *typeOpt;

			std::cout << type->label << '\n';

			if (type->config.IsEmpty())
				return test::documents::find_hsv(maskedHSV, type->config);

			return ToHSVBinary(maskedHSV, type->config);
		}
		std::optional<cv::Mat> BoothData::ExtractFace() const
		{
			if (!applicantHeadshot)
				return std::nullopt;

			auto preprocessed = PreprocessApplicantHeadshot(applicantHeadshot.value());
			if (!preprocessed)
				return std::nullopt;

			const cv::Mat& headshot = preprocessed.value();

			auto boundingBoxOpt = documents::FindBoundingBox(headshot, [&](int row, int col)
			{
				return headshot.at<uchar>(row, col) == 0;
			});

			if (!boundingBoxOpt)
				return std::nullopt;

			cv::Mat originalCutout(applicantHeadshot.value(), *boundingBoxOpt);
			cv::Mat silhouetteMask(headshot, *boundingBoxOpt);

			cv::Mat hsvImage;
			cv::cvtColor(originalCutout, hsvImage, cv::COLOR_BGR2HSV);

			cv::Mat maskedHSV;
			cv::bitwise_and(hsvImage, hsvImage, maskedHSV, silhouetteMask ^ 255);

			auto typeOpt = DetectHeadType(maskedHSV);
			if (!typeOpt)
			{
				cv::imshow("Unknown Head Type", maskedHSV);
				cv::waitKey();
				return std::nullopt;
			}

			auto* type = *typeOpt;
			std::cout << type->label << '\n';

			cv::Mat rawFaceMask;
			if (type->config.IsEmpty())
			{
				rawFaceMask = test::documents::find_hsv(maskedHSV, type->config);
			}
			else
			{
				rawFaceMask = ToHSVBinary(maskedHSV, type->config);
			}

			auto headOpt = this->ExtractHead();
			if (!headOpt)
				return std::nullopt;

			const cv::Mat& headMask = headOpt.value();

			return ((rawFaceMask ^ 255) & headMask) ^ 255;
		}
		std::optional<cv::Mat> BoothData::ExtractBinaryHeadshot() const
		{
			auto silhouette = this->ExtractSilhouette();
			auto head = this->ExtractHead();
			auto face = this->ExtractFace();

			if (!head || !face)
				return silhouette;

			cv::Mat resultImg;
			cv::bitwise_and(*face, *face, resultImg, *head);

			if (silhouette)
			{
				cv::Mat combined;
				cv::bitwise_or(*silhouette, resultImg, combined);
				return combined;
			}

			return resultImg;
		}
		documents::data::Photo BoothData::PhotoToBinaryHeadshotPhoto() const
		{
			if (!applicantHeadshot)
				return documents::data::Photo{};

			return documents::data::Photo{ PreprocessApplicantHeadshot(applicantHeadshot.value()).value_or(cv::Mat{}) };
		}

		static std::optional<SIUnitValue> FindApplicantHeight(const cv::Mat& applicantInBooth)
		{
			auto preprocessed = PreprocessApplicantHeadshot(applicantInBooth);
			if (!preprocessed)
				return std::nullopt;

			const cv::Mat& headshot = preprocessed.value();

			auto boundingBoxOpt = documents::FindBoundingBox(headshot, [&](int row, int col)
			{
				return headshot.at<uchar>(row, col) == 0;
			});

			if (!boundingBoxOpt)
				return std::nullopt;

#if 0
			const int height = boundingBoxOpt->height;

			constexpr int minHeightCm = 100;

			// adjust bounding box height to include next height line
			constexpr scaled<int> spaceBetweenLines = 22; // Include one line in this number, whitespace between is 18
			int approximateHeight = minHeightCm + (height * 10 / spaceBetweenLines) + ((height % spaceBetweenLines) * 10 / spaceBetweenLines);
#else
			const int pixelHeight = boundingBoxOpt->height;

			// Calibration constants
			constexpr int minHeightCm = 100;                  // Base height (game logic start)
			constexpr int cmPerLine = 10;                     // Each gap = 10 cm
			constexpr scaled<int> lineSpacingPx = 22;         // Pixel spacing per height line

			int approximateHeight = minHeightCm + (pixelHeight * cmPerLine / lineSpacingPx);
#endif

			return SIUnitValue(approximateHeight, SIUnit::CM);
		}

#endif

#pragma endregion
		
		static DocData ExtractBoothData(const cv::Mat& booth)
		{
			auto binary = PrepareBoothForOCR(booth);
			static constinit auto boothLayout = documents::DocLayout::GetBooth();
			static constinit auto layouts = boothLayout.GetAllLayouts();

			documents::DocDataBuilder builder{};
			for (size_t i = 0; i < boothLayout.GetLayoutCount(); i++)
			{
				switch (layouts[i].GetType())
				{
					case documents::FieldType::Text:
					{
						std::string raw_data = GetBoothString(ExtractDocumentField(binary, layouts[i].GetBox()));
#if OPTIMIZE_DOCDATA
						builder.AddField(
							Field{
								Data{ std::move(raw_data) },
								layouts[i].GetType(),
								layouts[i].GetCategory()
							},
							layouts[i].GetCategory()
						);
#else
						builder.AddFieldData(
							layouts[i].GetCategory(),
							documents::Field{ documents::Data{ std::move(raw_data) },
							layouts[i].GetType(),
							layouts[i].GetCategory()
							}
						);
#endif
						break;
					}
					case documents::FieldType::Image:
					{
						cv::Mat image_data = ExtractDocumentField(booth, layouts[i].GetBox());
#if OPTIMIZE_DOCDATA
						builder.AddField(
							Field{
								Data{
									std::move(image_data)
								},
								layouts[i].GetType(),
								layouts[i].GetCategory()
							},
							layouts[i].GetCategory()
						);
#else
						builder.AddFieldData(
							layouts[i].GetCategory(),
							documents::Field{
								documents::Data{
									std::move(image_data)
								},
								layouts[i].GetType(),
								layouts[i].GetCategory()
							}
						);
#endif
						break;
					}
					case documents::FieldType::Invalid:
					default:
					{
#if OPTIMIZE_DOCDATA
						builder.AddField(
							Field{
								Data{},
								layouts[i].GetType(),
								layouts[i].GetCategory()
							},
							layouts[i].GetCategory()
						);
#else
						builder.AddFieldData(
							layouts[i].GetCategory(),
							documents::Field{
								documents::Data{},
								layouts[i].GetType(),
								layouts[i].GetCategory()
							}
						);
#endif
						break;
					}
				}
			}

#if OPTIMIZE_DOCDATA
			return builder.Build();
#else
			return { builder.GetDocData() };
#endif
		}
		
		BoothData ScanBooth(const GameView& gameView)
		{
			auto loaded = ExtractBoothData(gameView.booth());

			auto boothDate       = loaded.GetFieldData<FieldCategory::BoothDate>();
			auto weight          = loaded.GetFieldData<FieldCategory::Weight>();
			auto photo           = loaded.GetFieldData<FieldCategory::Photo>();
			auto counter         = loaded.GetFieldData<FieldCategory::BoothCounter>();
			auto applicantHeight = photo ? FindApplicantHeight(photo.value()) : std::nullopt;

			auto scanResult = ScanForDocuments(gameView, ViewArea::BoothView);

			return BoothData{
				boothDate,
				weight,
				applicantHeight,
				photo,
				counter,
				std::move(scanResult)
			};
		}

	}  // namespace scannable
}  // namespace paplease
