#pragma once
#include <optional>

#include "base/document_data/date.h"
#include "base/document_data/field_data.h"
#include "base/game_state/components/component.h"
//#include "base/document_data/photo.h"
//#include "base/documents_v2/doc_data.h"

namespace paplease {
	namespace analysis {
		namespace components {

struct BoothData
{
	const documents::data::Date date;
	const documents::data::SIUnitValue weight;
	// documents::data::SIUnitValue m_approximateHeight;
	// documents::data::Photo m_applicantHeadshot;
	const int applicantNumber;
};


class BoothComponent final// : public Component
{
public:
	//using Component::Component;

	std::optional<BoothData> Scan(const cv::Mat& booth) const;
};

		}  // namespace components
	}  // namespace analysis
}  // namespace paplease
