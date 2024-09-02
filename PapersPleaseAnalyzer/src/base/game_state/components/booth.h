#pragma once
#include <optional>

#include "base/document_data/date.h"
#include "base/document_data/field_data.h"
#include "base/game_state/components/component.h"
//#include "base/document_data/photo.h"
//#include "base/documents_v2/doc_data.h"

struct BoothData
{
	const Documents::Data::Date date;
	const Documents::Data::SIUnitValue weight;
	// Documents::Data::SIUnitValue m_approximateHeight;
	// Documents::Data::Photo m_applicantHeadshot;
	const int applicantNumber;
};


class BoothComponent final// : public Component
{
public:
	//using Component::Component;

	std::optional<BoothData> Scan(const cv::Mat& booth) const;
};