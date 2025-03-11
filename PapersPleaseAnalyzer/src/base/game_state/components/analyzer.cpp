#include "pch.h"
#include "base/game_state/components/analyzer.h"

#include "base/documents_v2/seal.h"
#include "base/utils/log.h"

namespace paplease {
	namespace analysis {
		namespace components {

			using namespace documents::v2;

			bool DocumentAnalyzer::DocumentHasCurrentDate(const Doc& document) const
			{
				auto gameDate = m_mediator->RequestCurrentDate(); // Should never fail, since this will only run if UpdateBooth is successful

				// Get document expiration date
				auto data = document.GetDocumentData();
				auto expirationFieldData = data.Get(DataFieldCategory::ExpirationDate);
				if (expirationFieldData.Type() == documents::v2::FieldType::Invalid)
				{
					return true; // Maybe?
				}

				const auto& dateData = expirationFieldData.GetData();
				if (dateData.Broken())
				{
					LOG_ERR("Document has broken data!");
					return false;
				}

				const auto& date = dateData.Get<documents::data::Date>();

				// Compare dates
				return gameDate <= date;
			}

			Analysis DocumentAnalyzer::AnalyzeDocumentValidity(const documents::v2::Doc& document) const
			{
				bool isInvalid = false;
				isInvalid |= !this->ValidateSeal(document);
				isInvalid |= !this->ValidateDate(document);

				return { isInvalid };
			}

			bool DocumentAnalyzer::ValidateSeal(const documents::v2::Doc& document) const
			{
				bool hasValidSeal = document.IsAuthentic();
				if (!hasValidSeal)
				{
					LOG("Document has invalid seal");
					return false;
				}
				return true;
			}

			bool DocumentAnalyzer::ValidateDate(const documents::v2::Doc& document) const
			{
				bool hasCurrentDate = this->DocumentHasCurrentDate(document);
				if (!hasCurrentDate)
				{
					LOG("Document date has expired");
					return false;
				}
				return true;
			}


		}  // namespace components
	}  // namespace analysis
}  // namespace paplease
