#pragma once
#include "paplease/game_view.h"
#include "paplease/scannable/doc_scan.h"

namespace paplease {
	namespace scannable {

		struct InspectionData
		{
			DocViewCollection scannedDocuments;
		};

		InspectionData ScanInspection(const GameView& gameView);

	}  // namespace scannable
}  // namespace paplease
