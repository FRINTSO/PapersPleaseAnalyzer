#include "pch.h"
#include "base/analysis/scannable/inspection.h"

#include <utility>

namespace paplease {
	namespace analysis {
		namespace scannable {	

			using namespace documents;

			std::optional<InspectionData> ScanInspection(const GameView& gameView)
			{
				BeginLOG("ScanInspection()");
				std::array<Doc, InspectionData::DocumentScanCapacity> foundDocuments;
				size_t count = 0;

				for (auto documentType : GetDocTypeIterator())
				{
					auto document = FindDocument(gameView, documentType);
					if (!document) continue;

					assert(count < InspectionData::DocumentScanCapacity);
					foundDocuments[count] = std::move(document.value());
					count++;
				}

				EndLOG("ScanInspection()");
				if (count == 0)
				{
					return std::nullopt;
				}
				return InspectionData{
					foundDocuments
				};
			}

		}  // namespace scannable
	}  // namespace analysis
}  // namespace paplease
