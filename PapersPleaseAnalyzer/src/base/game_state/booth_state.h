#pragma once
#include "base/document_data/date.h"
#include "base/document_data/field_data.h"
#include "base/document_data/photo.h"
#include "base/documents_v2/doc_data.h"
#include "base/game_view.h"

class BoothDoc;


class BoothState
{
public:

	void Update(const GameView& gameView);


	const BoothDoc& GetNextAvailableDocument();
	Documents::Data::Date GetCurrentDate();
	Documents::Data::SIUnitValue GetCurrentWeight();
	Documents::Data::SIUnitValue GetApproximateHeight();
	Documents::Data::Photo GetApplicantSnapshot();
	int GetCurrentApplicantNumber();
private:
	void LoadBoothData(const cv::Mat& booth);
	// Documents::V2::DocData ExtractBoothData(const cv::Mat& booth);
private:
	struct BoothData
	{
		Documents::Data::Date m_date;
		Documents::Data::SIUnitValue m_weight;
		// Documents::Data::SIUnitValue m_approximateHeight;
		// Documents::Data::Photo m_applicantHeadshot;
		int m_applicantNumber;
	};
private:
	BoothData m_data;
	BoothData m_previousData;
};