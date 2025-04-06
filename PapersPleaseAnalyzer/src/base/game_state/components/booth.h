#pragma once
#include <optional>

#include "base/document_data/date.h"
#include "base/document_data/field_data.h"

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


			class BoothComponent final
			{
			public:
				std::optional<BoothData> Scan(const cv::Mat& booth) const;
			};

		}  // namespace components
	}  // namespace analysis
}  // namespace paplease
