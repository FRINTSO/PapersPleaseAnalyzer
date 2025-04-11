#include "pch.h"
#include "base/analysis/scannable/booth.h"

#include <opencv2/opencv.hpp>

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
				cv::Mat imgHsv;
				cv::cvtColor(booth, imgHsv, cv::COLOR_BGR2HSV);

				constexpr int hueMin = 12;
				constexpr int hueMax = 18;
				constexpr int satMin = 0;
				constexpr int satMax = 255;
				constexpr int valMin = 106;
				constexpr int valMax = 106;
				cv::Mat lower{ hueMin, satMin, valMin };
				cv::Mat upper{ hueMax, satMax, valMax };

				cv::Mat mask;
				cv::inRange(imgHsv, lower, upper, mask);

				cv::Mat insetImage(mask, cv::Rect(DOWNSCALE(168), DOWNSCALE(416), DOWNSCALE(22), DOWNSCALE(12)));
				cv::Mat alter = (insetImage ^ 255);
				alter.copyTo(insetImage);

				return mask;
			}

			static std::optional<DocData> ExtractBoothData(const cv::Mat& booth)
			{
				auto binary = PreprocessBooth(booth);
				static constexpr auto boothLayout = documents::DocLayout::GetBooth();
				auto layouts = boothLayout.GetAllLayouts();

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
				if (!data)
				{
					return std::nullopt;
				}
				return { data.value() };
			}

#pragma endregion

			static cv::Mat LoadHeadshotMask()
			{
				return ReadImage("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\booth\\headshot_mask.png") ^ 255;
				//return cv::imread(, cv::IMREAD_UNCHANGED) ^ 255;
			}

			static cv::Mat LoadHeightChart()
			{
				return ReadImage("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\booth\\headshot_height_chart.png");
				//return cv::imread(, cv::IMREAD_UNCHANGED);
			}

			static std::optional<cv::Mat> PreprocessApplicantHeadshot(const cv::Mat& applicantInBooth)
			{
				// White text
				//constexpr HSVConfig exactTextColorToWhite{ 117, 117, 45, 45, 63, 63};
				constexpr HSVConfig isHeadshotScanReadyConfig{ 10, 179, 0, 60, 53, 100 };
				bool isHeadshotReady = cv::countNonZero(ApplyHSV(applicantInBooth, isHeadshotScanReadyConfig)) == 0;
				if (!isHeadshotReady)
				{
					return std::nullopt;
				}

				// Extract headshot black silhouette + height measurements + black text
				//constexpr HSVConfig headshotHsvConfig{ 0, 23, 1, 70, 0, 255 };
				constexpr HSVConfig headshotHsvConfig{ 0, 23, 1, 71, 0, 255 };
				auto binaryHeadshot = ApplyHSV(applicantInBooth, headshotHsvConfig);

				static const cv::Mat headshotMask = LoadHeadshotMask();

				binaryHeadshot ^= headshotMask;

				if (cv::countNonZero(binaryHeadshot ^ 255) == 0)
				{
					return std::nullopt;
				}
				return binaryHeadshot;
			}

			static cv::Mat CutoutHeadshot(const cv::Mat& headshot)
			{
				// Start at bottom and make a pivot in the middle
				// Widest is in the shoulders

				Rectangle headshotBoundingBox = documents::FindBoundingBox(headshot, [&](int row, int col)
				{
					return headshot.at<uchar>(row, col) == 0;
				});

				// adjust bounding box height to include next height line
				constexpr int heightToTopLine = DOWNSCALE(198);
				constexpr int spaceBetweenLines = DOWNSCALE(22); // Include one line in this number, whitespace between is 18

				int newHeight = std::min((headshotBoundingBox.height / spaceBetweenLines + 1) * spaceBetweenLines, headshot.rows);
				int approximateHeight = 100 + (headshotBoundingBox.height * 10 / spaceBetweenLines) + ((headshotBoundingBox.height % spaceBetweenLines) * 10 / spaceBetweenLines);
				std::cout << "Height: " << approximateHeight << "\n";

				headshotBoundingBox.y = headshot.rows - newHeight;
				headshotBoundingBox.height = newHeight;

				static const cv::Mat heightChart = LoadHeightChart();

				cv::Mat inset(headshot & heightChart, cv::Rect(headshotBoundingBox.x, headshotBoundingBox.y, headshotBoundingBox.width, headshotBoundingBox.height));
				//cv::imshow("inset", inset);
				//cv::waitKey();

				return cv::Mat{};
			}

			static std::optional<SIUnitValue> FindApplicantHeight(const documents::data::Photo& applicantInBooth)
			{
				auto mat = PreprocessApplicantHeadshot(applicantInBooth.m_mat);
				if (!mat) return std::nullopt;
				//cv::imshow("Applicant height, headshot", mat.value());
				//cv::waitKey();

				// Make a smaller cutout
				// CutoutHeadshot(mat.value());

				// Calculate approximate height
				Rectangle headshotBoundingBox = documents::FindBoundingBox(mat.value(), [&](int row, int col)
				{
					return mat->at<uchar>(row, col) == 0;
				});

				// adjust bounding box height to include next height line
				constexpr int heightToTopLine = DOWNSCALE(198);
				constexpr int spaceBetweenLines = DOWNSCALE(22); // Include one line in this number, whitespace between is 18
				int approximateHeight = 100
					+ (headshotBoundingBox.height * 10 / spaceBetweenLines)
					+ ((headshotBoundingBox.height % spaceBetweenLines) * 10 / spaceBetweenLines);
				
				return SIUnitValue(approximateHeight, SIUnit::CM);
			}

			std::optional<BoothData> ScanBooth(const GameView& gameView)
			{
				BeginLOG("ScanBooth()");
				auto loadedData = ExtractBoothData(gameView.booth);
				if (!loadedData)
				{
					LOG_ERR("Booth data couldn't be loaded!");
					EndLOG("ScanBooth()");
					return std::nullopt;
				}

				EndLOG("ScanBooth()");
				return BoothData{
					loadedData->GetFieldData<FieldCategory::BoothDate>()->get(),
					loadedData->GetFieldData<FieldCategory::Weight>()->get(),
					FindApplicantHeight(loadedData->GetFieldData<FieldCategory::Photo>()->get()),
					loadedData->GetFieldData<FieldCategory::Photo>()->get(),
					loadedData->GetFieldData<FieldCategory::BoothCounter>()->get()
				};
			}

		}  // namespace scannable
	}  // namespace analysis
}  // namespace paplease
