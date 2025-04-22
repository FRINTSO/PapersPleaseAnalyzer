#include "pch.h"
#include "base/analysis/scannable/booth.h"

#include <opencv2/opencv.hpp>

#include "base/color.h"
#include "base/documents/doc_data.h"
#include "base/documents/doc_layout.h"
#include "base/image_process.h"
#include "base/utils/log.h"
#include "test/documents/test_hsv.h"

#include "base/documents/bounding_box_finder.inc"

namespace paplease {
	namespace analysis {
		namespace scannable {

			using namespace documents;
			using namespace documents::data;

#pragma region Extract Data

			static cv::Mat PreprocessBooth(const cv::Mat& booth)
			{
				static constinit HSVConfig config{ 12, 18, 0, 255, 106, 106 };
				auto mask = ToHSVBinary(booth, config);
				mask(ScaledRectangle(168, 416, 22, 12)) ^= 255;

				return mask;
			}

			static DocData ExtractBoothData(const cv::Mat& booth)
			{
				auto binary = PreprocessBooth(booth);
				static constinit auto boothLayout = documents::DocLayout::GetBooth();
				static constinit auto layouts = boothLayout.GetAllLayouts();

				documents::DocDataBuilder builder{};
				for (size_t i = 0; i < boothLayout.GetLayoutCount(); i++)
				{
					switch (layouts[i].GetType())
					{
						case documents::FieldType::Text:
						{
							auto raw_data = GetBoothString(ExtractDocumentField(binary, layouts[i].GetBox()));
							builder.AddFieldData(layouts[i].GetCategory(), documents::Field{ documents::Data{ std::move(raw_data) }, layouts[i].GetType(), layouts[i].GetCategory() });
							break;
						}
						case documents::FieldType::Image:
						{
							auto&& image_data = ExtractDocumentField(booth, layouts[i].GetBox());

							builder.AddFieldData(
								layouts[i].GetCategory(),
								documents::Field{
									documents::Data{
										Photo{ std::move(image_data) }
									},
									layouts[i].GetType(),
									layouts[i].GetCategory()
								}
							);
							break;
						}
						case documents::FieldType::Invalid:
						default:
						{
							builder.AddFieldData(layouts[i].GetCategory(), documents::Field{ documents::Data{}, layouts[i].GetType(), layouts[i].GetCategory() });
							break;
						}
					}
				}

				auto data = builder.GetDocData();
				return { data };
			}

#pragma endregion

			static cv::Mat LoadHeadshotMask()
			{
				return LoadImageFile("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\booth\\headshot_mask.png") ^ 255;
			}

			static cv::Mat LoadHeightChart()
			{
				return LoadImageFile("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\booth\\headshot_height_chart.png");
			}

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

			static std::optional<SIUnitValue> FindApplicantHeight(const documents::data::Photo& applicantInBooth)
			{
				auto preprocessed = PreprocessApplicantHeadshot(applicantInBooth.m_mat);
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

				auto preprocessed = PreprocessApplicantHeadshot(applicantHeadshot->m_mat);
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

				auto preprocessed = PreprocessApplicantHeadshot(applicantHeadshot->m_mat);
				if (!preprocessed)
					return std::nullopt;

				const cv::Mat& headshot = preprocessed.value();

				auto boundingBoxOpt = documents::FindBoundingBox(headshot, [&](int row, int col)
				{
					return headshot.at<uchar>(row, col) == 0;
				});

				if (!boundingBoxOpt)
					return std::nullopt;

				cv::Mat originalCutout(applicantHeadshot->m_mat, *boundingBoxOpt);
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

				auto preprocessed = PreprocessApplicantHeadshot(applicantHeadshot->m_mat);
				if (!preprocessed)
					return std::nullopt;

				const cv::Mat& headshot = preprocessed.value();

				auto boundingBoxOpt = documents::FindBoundingBox(headshot, [&](int row, int col)
				{
					return headshot.at<uchar>(row, col) == 0;
				});

				if (!boundingBoxOpt)
					return std::nullopt;

				cv::Mat originalCutout(applicantHeadshot->m_mat, *boundingBoxOpt);
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

				return documents::data::Photo{ PreprocessApplicantHeadshot(applicantHeadshot->m_mat).value_or(cv::Mat{}) };
			}

			BoothData ScanBooth(const GameView& gameView)
			{
				auto loaded = ExtractBoothData(gameView.booth);

				auto boothDate = loaded.GetFieldData<FieldCategory::BoothDate>();
				auto weight    = loaded.GetFieldData<FieldCategory::Weight>();
				auto photo     = loaded.GetFieldData<FieldCategory::Photo>();
				auto counter   = loaded.GetFieldData<FieldCategory::BoothCounter>();
				
				return BoothData{
					boothDate,
					weight,
					photo ? FindApplicantHeight(*photo) : std::nullopt,
					photo,
					counter
				};
			}

		}  // namespace scannable
	}  // namespace analysis
}  // namespace paplease
