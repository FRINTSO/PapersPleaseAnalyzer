#pragma once
#include <optional>

#include "base/documents/data/date.h"
#include "base/documents/data/field_data.h"
#include "base/documents/data/photo.h"
#include "base/game_view.h"

namespace paplease {
	namespace analysis {
		namespace scannable {

			struct BoothData
			{
				const documents::data::Date date;
				const documents::data::SIUnitValue weight;
				const std::optional<documents::data::SIUnitValue> m_approximateHeight;
				const documents::data::Photo m_applicantHeadshot;
				const int applicantNumber;
			};

			std::optional<BoothData> ScanBooth(const GameView& gameView);

		}  // namespace scannable
	}  // namespace analysis
}  // namespace paplease
