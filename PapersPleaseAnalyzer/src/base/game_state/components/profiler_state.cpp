#include "pch.h"
#include "base/game_state/components/profiler_state.h"

#include "base/utils/log.h"

namespace paplease {
	namespace analysis {
		namespace components {

using namespace documents::v2;

void ProfilerState::Update()
{
	 BeginLOG("ProfilerState::Update()");

	// establish nationality
	// establish id-number
	// establish dob
	// establish name

	EndLOG("ProfilerState::Update()");
	// Do comparison and build profile
}

void ProfilerState::ReceiveApplicantDocument(const documents::v2::Doc& document)
{
	// Check document authenticity
	document.IsAuthentic();

	this->RegisterDocument(document);
}

void ProfilerState::RegisterDocument(const documents::v2::Doc& document)
{
	assert(m_documentCount + 1 < ProfilerState::DocumentCapacity);

	LOG("Registered a document to Profiler!");

	m_comparableDocuments[m_documentCount] = document;
	m_documentCount++;
}

std::array<DataFieldCategory, Profile::FieldsCapacity> Profile::ProfileFields()
{
	// Choose what fields should be used
	return {
		DataFieldCategory::Nationality,
		DataFieldCategory::DateOfBirth,
		DataFieldCategory::Height,
		DataFieldCategory::IssuingCity,
		DataFieldCategory::IssuingCountry,
		DataFieldCategory::Name,
		DataFieldCategory::PassportNumber,
		DataFieldCategory::Sex,
		DataFieldCategory::Vaccination1,
		DataFieldCategory::Vaccination2,
		DataFieldCategory::Vaccination3,
		DataFieldCategory::Weight,
	};
}

int Profile::GetFieldIndexByCategoryType(documents::v2::DataFieldCategory category)
{
	switch (category)
	{
		case DataFieldCategory::Nationality:
			return 0;
		case DataFieldCategory::DateOfBirth:
			return 1;
		case DataFieldCategory::Height:
			return 2;
		case DataFieldCategory::IssuingCity:
			return 3;
		case DataFieldCategory::IssuingCountry:
			return 4;
		case DataFieldCategory::Name:
			return 5;
		case DataFieldCategory::PassportNumber:
			return 6;
		case DataFieldCategory::Sex:
			return 7;
		case DataFieldCategory::Vaccination1:
			return 8;
		case DataFieldCategory::Vaccination2:
			return 9;
		case DataFieldCategory::Vaccination3:
			return 10;
		case DataFieldCategory::Weight:
			return 11;
		default:
			return -1;
	}
}

		}  // namespace components
	}  // namespace analysis
}  // namespace paplease
