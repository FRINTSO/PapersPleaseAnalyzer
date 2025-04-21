#include "pch.h"
#include "base/analysis/scannable/inspection.h"

#include <utility>

#include "base/utils/enum_range.h"

namespace paplease {
	namespace analysis {
		namespace scannable {	

			using namespace documents;

			InspectionData ScanInspection(const GameView& gameView)
			{
				std::array<Doc, InspectionData::DocumentScanCapacity> foundDocuments;
				size_t count = 0;

				for (auto documentType : utils::enum_range(DocType::AccessPermit, DocType::Passport))
				{
					auto document = FindDocument(gameView, documentType);
					if (!document) continue;
					if (document->GetDocumentData().HasBrokenData())
					{
						LOG_ERR("Broken document '{}'", ToStringView(document->GetDocumentType()));
						continue;
					}

					assert(count < InspectionData::DocumentScanCapacity);
					foundDocuments[count] = std::move(document.value());
					count++;
				}

				return InspectionData{
					foundDocuments
				};
			}

		}  // namespace scannable
	}  // namespace analysis
}  // namespace paplease
