#include "pch.h"
#include "base/game_state/components/analyzer.h"

#include "base/documents_v2/seal.h"
#include "base/utils/log.h"

using namespace Documents::V2;

bool Analyzer::DocumentHasCurrentDate(const Doc& document) const
{
	const auto& gameDate = m_mediator->RequestCurrentDate();
	// Date found do this:

	// Get Date
	auto data = document.GetDocumentData();
	auto expirationFieldData = data.Get(DataFieldCategory::ExpirationDate);
	const auto& dateData = expirationFieldData.GetData();
	if (dateData.Broken())
	{
		LOG_ERR("Document has broken data!");
		return false;
	}

	const auto& date = dateData.Get<Documents::Data::Date>();

	// Compare dates

	return gameDate <= date;
}

Analysis Analyzer::AnalyzeDocumentValidity(const Documents::V2::Doc& document) const
{
	bool hasValidSeal = document.IsAuthentic();
	if (!hasValidSeal)
	{
		LOG("Document has invalid seal");
	}

	bool hasCurrentDate = this->DocumentHasCurrentDate(document);
	if (!hasCurrentDate)
	{
		LOG("Document date has expired");
	}
	return {};
}