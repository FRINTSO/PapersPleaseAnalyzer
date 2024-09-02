#include "pch.h"
#include "base/game_state/components/booth.h"

#include <opencv2/opencv.hpp>

#include "base/documents/document.h"
#include "base/documents_v2/doc_data.h"
#include "base/documents_v2/doc_layout.h"
#include "base/image_process.h"
#include "base/utils/log.h"

using namespace Documents::Data;
using namespace Documents::V2;

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
	static constexpr auto boothLayout = Documents::V2::DocLayout::GetBooth();
	auto layouts = boothLayout.GetAllLayouts();

	Documents::V2::DocDataBuilder builder{};

	for (size_t i = 0; i < boothLayout.GetLayoutCount(); i++)
	{
		switch (layouts[i].GetType())
		{
			case Documents::V2::FieldType::Text:
			{
				auto raw_data = GetFieldString(ExtractDocumentField(binary, layouts[i].GetBox()), Documents::V1::DocumentType::Booth);
				builder.AddFieldData(layouts[i].GetCategory(), Documents::V2::FieldData{ Documents::V2::Data{ raw_data }, layouts[i].GetType(), layouts[i].GetCategory() });
				break;
			}
			case Documents::V2::FieldType::Image:
			case Documents::V2::FieldType::Invalid:
			default:
			{
				builder.AddFieldData(layouts[i].GetCategory(), Documents::V2::FieldData{ Documents::V2::Data{}, layouts[i].GetType(), layouts[i].GetCategory() });
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
