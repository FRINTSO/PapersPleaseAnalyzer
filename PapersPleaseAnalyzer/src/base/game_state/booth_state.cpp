#include "pch.h"
#include "base/game_state/booth_state.h"

#include <iostream>

#include <opencv2/opencv.hpp>

#include "base/image_process.h"
#include "base/documents/document.h"
#include "base/documents_v2/doc_data.h"
#include "base/documents_v2/doc_layout.h"

using namespace Documents::Data;
using namespace Documents::V2;

void BoothState::Update(const GameView& gameView)
{
	std::cout << "Begin Booth Update\n";
	this->LoadBoothData(gameView.booth);

	// If new date, then game needs to update state of rules, criminals
	if (m_data.m_date != m_previousData.m_date)
	{
		std::cout << "New Date!\n";
	}

	if (m_data.m_applicantNumber != m_previousData.m_applicantNumber)
	{
		std::cout << "New Applicant!\n";
	}

	std::cout << "End Booth Update\n";
}

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

static DocData ExtractBoothData(const cv::Mat& booth)
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
	return { builder.GetDocData() };
}

void BoothState::LoadBoothData(const cv::Mat& booth)
{
	m_previousData = m_data;
	auto loadedData = ExtractBoothData(booth);
	m_data.m_date = loadedData.Get(DataFieldCategory::BoothDate).GetData().Get<Date>();
	m_data.m_weight = loadedData.Get(DataFieldCategory::Weight).GetData().Get<SIUnitValue>();
	m_data.m_applicantNumber = loadedData.Get(DataFieldCategory::BoothCounter).GetData().Get<long long>();
}