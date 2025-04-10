#include "pch.h"
#include "base/analysis/scannable/booth.h"

#include <opencv2/opencv.hpp>

#include "base/documents/doc_data.h"
#include "base/documents/doc_layout.h"
#include "base/image_process.h"
#include "base/utils/log.h"

namespace paplease {
	namespace analysis {
		namespace scannable {

			using namespace documents::data;
			using namespace documents;

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
							builder.AddFieldData(layouts[i].GetCategory(), documents::Field{ documents::Data{ raw_data }, layouts[i].GetType(), layouts[i].GetCategory() });
							break;
						}
						case documents::FieldType::Image:
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
					loadedData->GetField(FieldCategory::BoothDate).GetData().Get<Date>(),
					loadedData->GetField(FieldCategory::Weight).GetData().Get<SIUnitValue>(),
					loadedData->GetField(FieldCategory::BoothCounter).GetData().Get<int>()
				};
			}

		}  // namespace scannable
	}  // namespace analysis
}  // namespace paplease
