#pragma once

namespace paplease {
	namespace analysis_v1 {

		constexpr std::array<documents::FieldCategory, Profile::FieldsCapacity> Profile::ProfileFields()
		{
			return {
				//documents::DataFieldCategory::Nationality,
				documents::FieldCategory::DateOfBirth,
				documents::FieldCategory::Height,
				documents::FieldCategory::IssuingCity,
				documents::FieldCategory::IssuingCountry,
				documents::FieldCategory::Name,
				documents::FieldCategory::PassportNumber,
				documents::FieldCategory::Sex,
				documents::FieldCategory::Vaccination1,
				documents::FieldCategory::Vaccination2,
				documents::FieldCategory::Vaccination3,
				documents::FieldCategory::Weight,
			};
		}

		constexpr int Profile::GetFieldIndexByCategoryType(documents::FieldCategory category)
		{
			switch (category)
			{
				//case documents::DataFieldCategory::Nationality:
				//	return 0;
				case documents::FieldCategory::DateOfBirth:
					return 0;
				case documents::FieldCategory::Height:
					return 1;
				case documents::FieldCategory::IssuingCity:
					return 2;
				case documents::FieldCategory::IssuingCountry:
					return 3;
				case documents::FieldCategory::Name:
					return 4;
				case documents::FieldCategory::PassportNumber:
					return 5;
				case documents::FieldCategory::Sex:
					return 6;
				case documents::FieldCategory::Vaccination1:
					return 7;
				case documents::FieldCategory::Vaccination2:
					return 8;
				case documents::FieldCategory::Vaccination3:
					return 9;
				case documents::FieldCategory::Weight:
					return 10;
				default:
					assert(false && "Shouldn't pass this value");
					return -1;
			}
		}

	}  // namespace analysis_v1
}  // namespace paplease
