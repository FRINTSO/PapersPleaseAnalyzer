#pragma once
#include "base/analysis/scannable/booth.h"
#include "base/analysis/scannable/inspection.h"
#include "base/game_view.h"

#include <optional>

namespace paplease {
	namespace analysis {
		namespace scannable {
			
			struct ScanContext
			{
				std::optional<scannable::BoothData> boothData;
				std::optional<scannable::InspectionData> inspectionData;
			};

			std::optional<scannable::ScanContext> Scan(const GameView& gameView);

		}  // namespace scannable
	}  // namespace analysis
}  // namespace paplease
