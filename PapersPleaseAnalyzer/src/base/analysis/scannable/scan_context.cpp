#include "pch.h"
#include "base/analysis/scannable/scan_context.h"

namespace paplease {
	namespace analysis {
		namespace scannable {

			scannable::ScanContext Scan(const GameView& gameView)
			{
				// Scanning
				auto boothData = scannable::ScanBooth(gameView);  // Gets game state and some applicant info
				auto inspectionData = scannable::ScanInspection(gameView);  // Gets documents

				return scannable::ScanContext{
					boothData,
					inspectionData
				};
			}

		}  // namespace scannable
	}  // namespace analysis
}  // namespace paplease
