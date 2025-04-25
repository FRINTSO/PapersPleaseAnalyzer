#pragma once
#include "paplease/scannable/doc_scan.h"
#include "paplease/game_view.h"

namespace paplease {
	namespace scannable {

		struct InspectionData
		{
			DocViewCollection scannedDocuments;
		};

		InspectionData ScanInspection(const GameView& gameView);

	}  // namespace scannable
}  // namespace paplease
