#pragma once

namespace paplease {
	namespace analysis {

		constexpr std::array<documents::DataFieldCategory, Profile::FieldsCapacity> Profile::ProfileFields()
		{
			return {
				//documents::DataFieldCategory::Nationality,
				documents::DataFieldCategory::DateOfBirth,
				documents::DataFieldCategory::Height,
				documents::DataFieldCategory::IssuingCity,
				documents::DataFieldCategory::IssuingCountry,
				documents::DataFieldCategory::Name,
				documents::DataFieldCategory::PassportNumber,
				documents::DataFieldCategory::Sex,
				documents::DataFieldCategory::Vaccination1,
				documents::DataFieldCategory::Vaccination2,
				documents::DataFieldCategory::Vaccination3,
				documents::DataFieldCategory::Weight,
			};
		}

		constexpr int Profile::GetFieldIndexByCategoryType(documents::DataFieldCategory category)
		{
			switch (category)
			{
				//case documents::DataFieldCategory::Nationality:
				//	return 0;
				case documents::DataFieldCategory::DateOfBirth:
					return 0;
				case documents::DataFieldCategory::Height:
					return 1;
				case documents::DataFieldCategory::IssuingCity:
					return 2;
				case documents::DataFieldCategory::IssuingCountry:
					return 3;
				case documents::DataFieldCategory::Name:
					return 4;
				case documents::DataFieldCategory::PassportNumber:
					return 5;
				case documents::DataFieldCategory::Sex:
					return 6;
				case documents::DataFieldCategory::Vaccination1:
					return 7;
				case documents::DataFieldCategory::Vaccination2:
					return 8;
				case documents::DataFieldCategory::Vaccination3:
					return 9;
				case documents::DataFieldCategory::Weight:
					return 10;
				default:
					return -1;
			}
		}

	}  // namespace analysis
}  // namespace paplease
