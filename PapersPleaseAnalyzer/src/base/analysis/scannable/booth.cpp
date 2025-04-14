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
				//static HSVConfig config{};
				//tests::documents::find_hsv(applicantInBooth, config);

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


			cv::Mat BoothData::ToSilhouette() const
			{
				auto headshot = PreprocessApplicantHeadshot(this->applicantHeadshot.m_mat).value_or(cv::Mat{});
				Rectangle box = documents::FindBoundingBox(headshot, [&](int row, int col)
				{
					return headshot.at<uchar>(row, col) == 0;
				});

				return cv::Mat(headshot, cv::Rect(box.x, box.y, box.width, box.height));
			}
			cv::Mat BoothData::ExtractHead() const
			{
				const cv::Mat& original = this->applicantHeadshot.m_mat;
				auto headshot_optional = PreprocessApplicantHeadshot(original);
				if (!headshot_optional.has_value()) return {};
				cv::Mat headshot = headshot_optional.value();
				Rectangle box = documents::FindBoundingBox(headshot, [&](int row, int col)
				{
					return headshot.at<uchar>(row, col) == 0;
				});

				cv::Mat cutoutOriginal(original, cv::Rect(box.x, box.y, box.width, box.height));
				cv::Mat headshotOriginal(headshot, cv::Rect(box.x, box.y, box.width, box.height));

				cv::Mat hsvImage;
				cv::cvtColor(cutoutOriginal, hsvImage, cv::COLOR_BGR2HSV);

				cv::Mat finalImage;
				cv::bitwise_and(hsvImage, hsvImage, finalImage, headshotOriginal ^ 255);

				HSVConfig* selectedConfig = nullptr;
				cv::Mat returnImage;

				for (size_t row = 0; row < finalImage.rows; row++)
				{
					for (size_t col = 0; col < finalImage.cols; col++)
					{
						const int bgr_val = BGR_VAL(*finalImage.ptr<BgrColor>(row, col));

						switch (bgr_val)
						{
							case rgb_color_v<67, 118, 0>:
							{
								std::cout << "Type 1\n";
								static HSVConfig typeConfig{1, 179, 0, 255, 0, 255};
								selectedConfig = &typeConfig;
								goto RETURN;
							}
							case rgb_color_v<87, 199, 11>:
							{
								std::cout << "Type 2\n";
								static HSVConfig typeConfig{1, 121, 0, 255, 0, 255};
								selectedConfig = &typeConfig;
								goto RETURN;
							}
							case rgb_color_v<91, 188, 8>:
							{
								std::cout << "Type 3\n";
								static HSVConfig typeConfig{0, 179, 126, 255, 0, 255};
								selectedConfig = &typeConfig;
								goto RETURN;
							}
							case rgb_color_v<98, 83, 105>:
							{
								std::cout << "Type 4\n";
								static HSVConfig typeConfig{66, 179, 0, 255, 0, 255};
								selectedConfig = &typeConfig;
								goto RETURN;
							}
							case rgb_color_v<80, 96, 128>:
							{
								std::cout << "Type 5\n";
								static HSVConfig typeConfig{37, 179, 0, 255, 0, 255};
								selectedConfig = &typeConfig;
								goto RETURN;
							}
							case rgb_color_v<80, 61, 69>:
							{
								std::cout << "Type 6\n";
								static HSVConfig typeConfig{1, 179, 0, 251, 0, 255};
								selectedConfig = &typeConfig;
								goto RETURN;
							}
						}
					}
				}
			RETURN:
				if (selectedConfig == nullptr)
				{
					cv::imshow("HELP KWAJDOIAWHDOUAWBDAWOUDBAWDUOBWADUWABDWAUDBAWDU", finalImage);
					cv::waitKey();
					return {};
				}

				if (selectedConfig->IsEmpty())
				{
					return tests::documents::find_hsv(finalImage, *selectedConfig);
				}
				else
				{
					returnImage = ApplyHSV(finalImage, *selectedConfig);
					/*cv::imshow("Returned Image", returnImage);
					cv::waitKey();
					cv::destroyAllWindows();*/
					return returnImage;
				}

#if 0
				const cv::Mat& original = this->applicantHeadshot.m_mat;

				cv::Mat hsvImage;
				cv::cvtColor(original, hsvImage, cv::COLOR_BGR2HLS);

				cv::Mat channels[3];
				cv::split(hsvImage, channels);

				const cv::Mat& satChannel = channels[1];

				constexpr int satMin = 175;
				constexpr int satMax = 225;

				cv::Mat satMask;
				cv::inRange(satChannel, cv::Mat{ satMin }, cv::Mat{ satMax }, satMask);
				cv::Mat satResult;
				cv::bitwise_and(satChannel, satChannel, satResult, satMask);


				cv::imshow("Original", original);
				cv::imshow("Gray", ToGrayscale(original));

				// FIND LIGHTEST FACE COLOR
				uchar lightest = 0;
				for (size_t row = 0; row < satResult.rows; row++)
				{
					for (size_t col = 0; col < satResult.cols; col++)
					{
						uchar val;
						if ((val = satResult.at<uchar>(row, col)) != 0)
						{
							lightest = val;
							goto LIGHTEST_ADJACENT;
						}
					}
				}

				// FIND SECOND LIGHTEST FACE COLOR
				LIGHTEST_ADJACENT:
				uchar secondLightest = 0;
				for (size_t row = 0; row < satChannel.rows; row++)
				{
					for (size_t col = 0; col < satChannel.cols; col++)
					{
						if (satChannel.at<uchar>(row, col) == lightest)
						{
							// UP
							uchar up = row - 1 >= 0 ? satChannel.at<uchar>(row - 1, col) : 0;
							up = up != lightest ? up : 0;

							// RIGHT
							uchar right = col + 1 < satChannel.cols ? satChannel.at<uchar>(row, col + 1) : 0;
							right = right != lightest ? right : 0;

							// DOWN
							uchar down = row + 1 < satChannel.rows ? satChannel.at<uchar>(row + 1, col) : 0;
							down = down != lightest ? down : 0;

							// LEFT
							uchar left = col - 1 >= 0 ? satChannel.at<uchar>(row, col - 1) : 0;
							left = left != lightest ? left : 0;


							secondLightest = std::max({ up, right, down, left });
						}
					}
				}

				std::cout << "Second Lightest: " << (int)secondLightest << "\n";

				// EXTRACT SECOND LIGHTEST
				cv::Mat satMask2;
				cv::inRange(satChannel, cv::Mat{ secondLightest - 10 }, cv::Mat{ secondLightest + 10 }, satMask2);
				cv::Mat satResult2;
				cv::bitwise_and(satChannel, satChannel, satResult2, satResult2);
				cv::imshow("sat mask 2", satResult2);

				// FLOOD FILL
				cv::Point seed(100, 100); // Starting point for fill
				cv::Scalar fillColor(0, 0, 255); // Red (in BGR)

				// Optional: define color difference thresholds
				cv::Scalar loDiff(20, 20, 20); // allowable lower brightness/color diff
				cv::Scalar upDiff(20, 20, 20); // allowable upper brightness/color diff

				// Flags:
				// - 4-connected or 8-connected
				// - Fill mask bit flag
				int flags = 4 | cv::FLOODFILL_FIXED_RANGE;

				
				cv::floodFill(original, seed, fillColor, 0, loDiff, upDiff, flags);
				cv::imshow("Flood Filled", original);

				// SHOW REST
				cv::imshow("Sat channel", channels[1]);
				cv::imshow("Sat mask", satResult);
				cv::waitKey();
#endif
				return {};
			}
			cv::Mat BoothData::ExtractFace() const
			{
				const cv::Mat& original = this->applicantHeadshot.m_mat;
				auto headshot_optional = PreprocessApplicantHeadshot(original);
				if (!headshot_optional.has_value()) return {};
				cv::Mat headshot = headshot_optional.value();
				Rectangle box = documents::FindBoundingBox(headshot, [&](int row, int col)
				{
					return headshot.at<uchar>(row, col) == 0;
				});

				cv::Mat cutoutOriginal(original, cv::Rect(box.x, box.y, box.width, box.height));
				cv::Mat headshotOriginal(headshot, cv::Rect(box.x, box.y, box.width, box.height));

				cv::Mat hsvImage;
				cv::cvtColor(cutoutOriginal, hsvImage, cv::COLOR_BGR2HSV);

				cv::Mat finalImage;
				cv::bitwise_and(hsvImage, hsvImage, finalImage, headshotOriginal ^ 255);

				HSVConfig* selectedConfig = nullptr;
				cv::Mat returnImage;

				for (size_t row = 0; row < finalImage.rows; row++)
				{
					for (size_t col = 0; col < finalImage.cols; col++)
					{
						const int bgr_val = BGR_VAL(*finalImage.ptr<BgrColor>(row, col));

						switch (bgr_val)
						{
							case rgb_color_v<67, 118, 0>:
							{
								std::cout << "Type 1\n";
								static HSVConfig typeConfig{ 0, 42, 0, 255, 0, 255 };
								selectedConfig = &typeConfig;
								goto RETURN;
							}
							case rgb_color_v<87, 199, 11>:
							{
								std::cout << "Type 2\n";
								static HSVConfig typeConfig{ 0, 179, 0, 240, 0, 255 };
								selectedConfig = &typeConfig;
								goto RETURN;
							}
							case rgb_color_v<91, 188, 8>:
							{
								std::cout << "Type 3\n";
								static HSVConfig typeConfig{ 0, 179, 0, 243, 0, 255 };
								selectedConfig = &typeConfig;
								goto RETURN;
							}
							case rgb_color_v<98, 83, 105>:
							{
								std::cout << "Type 4\n";
								static HSVConfig typeConfig{ 0, 178, 0, 255, 113, 255 };
								selectedConfig = &typeConfig;
								goto RETURN;
							}
							case rgb_color_v<80, 96, 128>:
							{
								std::cout << "Type 5\n";
								static HSVConfig typeConfig{ 0, 179, 0, 255, 153, 255 };
								selectedConfig = &typeConfig;
								goto RETURN;
							}
							case rgb_color_v<80, 61, 69>:
							{
								std::cout << "Type 6\n";
								static HSVConfig typeConfig{ 0, 165, 0, 255, 0, 255 };
								selectedConfig = &typeConfig;
								goto RETURN;
							}
						}
					}
				}
			RETURN:
				if (selectedConfig == nullptr)
				{
					cv::imshow("HELP KWAJDOIAWHDOUAWBDAWOUDBAWDUOBWADUWABDWAUDBAWDU", finalImage);
					cv::waitKey();
					return {};
				}

				if (selectedConfig->IsEmpty())
				{
					return tests::documents::find_hsv(finalImage, *selectedConfig);
				}
				else
				{
					returnImage = ApplyHSV(finalImage, *selectedConfig);
					auto headImage = this->ExtractHead();
					/*cv::Mat finalImage2;
					cv::bitwise_and(returnImage ^ 255, headshotOriginal, finalImage2);*/

					//cv::imshow("Head original", headImage);
					//cv::imshow("Returned Image", returnImage);
					//cv::imshow("Inv Head original", headImage ^ 255);
					//cv::imshow("Inv Returned Image", returnImage ^ 255);
					//cv::imshow("Result Image", ((returnImage ^ 255) & headImage) ^ 255);
					//cv::waitKey();
					//cv::destroyAllWindows();
					return ((returnImage ^ 255) & headImage) ^ 255;
				}
			}
			documents::data::Photo BoothData::PhotoToBinaryHeadshotPhoto() const
			{
				return documents::data::Photo{ PreprocessApplicantHeadshot(this->applicantHeadshot.m_mat).value_or(cv::Mat{}) };
			}

			std::optional<BoothData> ScanBooth(const GameView& gameView)
			{
				auto loadedData = ExtractBoothData(gameView.booth);
				if (!loadedData)
				{
					LOG_ERR("Booth data couldn't be loaded!");
					return std::nullopt;
				}

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
