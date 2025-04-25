#pragma once
#include "paplease/scannable/booth.h"
#include "paplease/scannable/inspection.h"
#include "paplease/game_view.h"

namespace paplease {
	namespace scannable {
			
		struct ScanContext
		{
			const GameView* currentGameView;
			scannable::BoothData boothData;
			scannable::InspectionData inspectionData;
		};

		scannable::ScanContext Scan(const GameView& gameView);

	}  // namespace scannable
}  // namespace paplease
