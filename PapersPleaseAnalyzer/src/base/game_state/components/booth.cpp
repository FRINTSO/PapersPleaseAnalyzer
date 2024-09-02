#include "pch.h"
#include "base/game_state/components/booth.h"

#include <opencv2/opencv.hpp>

#include "base/documents_v2/doc_data.h"
#include "base/documents_v2/doc_layout.h"
#include "base/image_process.h"
#include "base/utils/log.h"

namespace paplease {
	namespace analysis {
		namespace components {

using namespace documents::data;
using namespace documents::v2;

#pragma region Extract Data

static cv::Mat PreprocessBooth(const cv::Mat& booth)
{
	cv::Mat imgHsv;
	cv::cvtColor(booth, imgHsv, cv::COLOR_BGR2HSV);

	const int hueMin = 12;
	const int hueMax = 18;
	const int satMin = 0;
	const int satMax = 255;
	const int valMin = 106;
	const int valMax = 106;
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
	static constexpr auto boothLayout = documents::v2::DocLayout::GetBooth();
	auto layouts = boothLayout.GetAllLayouts();

	documents::v2::DocDataBuilder builder{};

	for (size_t i = 0; i < boothLayout.GetLayoutCount(); i++)
	{
		switch (layouts[i].GetType())
		{
			case documents::v2::FieldType::Text:
			{
				auto raw_data = GetBoothString(ExtractDocumentField(binary, layouts[i].GetBox()));
				builder.AddFieldData(layouts[i].GetCategory(), documents::v2::FieldData{ documents::v2::Data{ raw_data }, layouts[i].GetType(), layouts[i].GetCategory() });
				break;
			}
			case documents::v2::FieldType::Image:
			case documents::v2::FieldType::Invalid:
			default:
			{
				builder.AddFieldData(layouts[i].GetCategory(), documents::v2::FieldData{ documents::v2::Data{}, layouts[i].GetType(), layouts[i].GetCategory() });
				break;
			}
		}
	}

	auto data = builder.GetDocData();
	if (!data)
	{
		return std::nullopt;
	}
	return { data.value()};
}

std::optional<BoothData> BoothComponent::Scan(const cv::Mat& booth) const
{
	auto loadedData = ExtractBoothData(booth);
	if (!loadedData)
	{
		LOG_ERR("Booth data couldn't be loaded!");
		return std::nullopt;
	}
	return BoothData{
		loadedData->Get(DataFieldCategory::BoothDate).GetData().Get<Date>(),
		loadedData->Get(DataFieldCategory::Weight).GetData().Get<SIUnitValue>(),
		loadedData->Get(DataFieldCategory::BoothCounter).GetData().Get<int>()
	};
}

#pragma endregion

		}  // namespace components
	}  // namespace analysis
}  // namespace paplease
