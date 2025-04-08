#include "pch.h"
#include "base/analysis/document_analyzer.h"

#include "base/documents/seal.h"

#include "base/utils/log.h"

namespace paplease {
	namespace analysis {
		
		using namespace documents;
		using namespace documents::data;

		static bool DocumentHasCurrentDate(const Doc& document, const Date& currentDate)
		{
			// auto gameDate = m_mediator->RequestCurrentDate(); // Should never fail, since this will only run if UpdateBooth is successful
			auto gameDate = currentDate;

			// Get document expiration date
			auto data = document.GetDocumentData();
			auto& expirationFieldData = data.GetField(DataFieldCategory::ExpirationDate);
			if (expirationFieldData.Type() == documents::FieldType::Invalid)
			{
				return true; // Maybe?
			}

			const auto& dateData = expirationFieldData.GetData();
			if (dateData.IsBroken())
			{
				LOG_ERR("Document has broken data!");
				return false;
			}

			const auto& date = dateData.Get<documents::data::Date>();

			// Compare dates
			return gameDate <= date;
		}

		static bool ValidateSeal(const documents::Doc& document)
		{
			bool hasValidSeal = document.IsAuthentic();
			if (!hasValidSeal)
			{
				LOG("Document has invalid seal");
				return false;
			}
			return true;
		}

		static bool ValidateDate(const documents::Doc& document, const Date& currentDate)
		{
			bool hasCurrentDate = DocumentHasCurrentDate(document, currentDate);
			if (!hasCurrentDate)
			{
				LOG("Document date has expired");
				return false;
			}
			return true;
		}

		DocAnalysis AnalyzeDocumentValidity(const documents::Doc& document, const Date& currentDate)
		{
			bool isInvalid = false;
			isInvalid |= !ValidateSeal(document);
			isInvalid |= !ValidateDate(document, currentDate);

			return { isInvalid };
		}

	}  // namespace analysis
}  // namespace paplease
