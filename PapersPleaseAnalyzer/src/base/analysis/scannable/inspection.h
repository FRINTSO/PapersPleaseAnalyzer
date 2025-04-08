#pragma once
#include <array>

#include "base/documents/doc_class.h"
#include "base/game_view.h"

namespace paplease {
	namespace analysis {
		namespace scannable {

			struct InspectionData
			{
				static constexpr size_t DocumentScanCapacity = 4;
				std::array<documents::Doc, DocumentScanCapacity> documents;
			};

			std::optional<InspectionData> ScanInspection(const GameView& gameView);

		}  // namespace scannable
	}  // namespace analysis
}  // namespace paplease
