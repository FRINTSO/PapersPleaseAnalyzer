#pragma once
#include "paplease/game_view.h"
#include "paplease/scannable/booth.h"
#include "paplease/scannable/inspection.h"

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
