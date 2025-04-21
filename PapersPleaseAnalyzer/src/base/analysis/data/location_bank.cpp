#include "pch.h"
#include "base/analysis/data/location_bank.h"

#include "base/utils/enum_range.h"
#include "base/utils/strfuncs.h"

namespace paplease {
	namespace analysis {
		namespace data {

			bool LocationBank::IsValidCountry(const std::string_view& countryName)
			{
				for (auto passportType : utils::enum_range(documents::PassportType::Antegria, documents::PassportType::UnitedFederation))
				{
					if (IsValidCountry(countryName, passportType))
					{
						return true;
					}
				}
				return false;
			}

			bool LocationBank::IsValidCountry(const std::string_view& countryName, documents::PassportType passportType)
			{
				return utils::strfuncs::ToLower(GetCountry(passportType)) == utils::strfuncs::ToLower(countryName);
			}

			bool LocationBank::IsValidDistrict(const std::string_view& districtName)
			{
				for (const auto& district : m_districtListLookUp)
				{
					if (utils::strfuncs::ToLower(district) == utils::strfuncs::ToLower(districtName))
					{
						return true;
					}
				}
				return false;
			}

			bool LocationBank::IsValidCity(const std::string_view& cityName)
			{
				for (auto passportType : utils::enum_range(documents::PassportType::Antegria, documents::PassportType::UnitedFederation))
				{
					if (IsValidCity(cityName, passportType))
					{
						return true;
					}
				}
				return false;
			}

			bool LocationBank::IsValidCity(const std::string_view& cityName, documents::PassportType passportType)
			{
				for (const auto& city : GetCityList(passportType))
				{
					if (utils::strfuncs::ToLower(city) == utils::strfuncs::ToLower(cityName))
					{
						return true;
					}
				}
				return false;
			}

			ECountry LocationBank::FromCountryString(const std::string_view& countryName)
			{
				for (size_t i = 0; i < Countries; ++i)
				{
					if (m_countryListLookUp[i] == countryName)
					{
						return static_cast<ECountry>(i + 1);  // Offset by 1 to skip ECountry::Invalid
					}
				}
				return ECountry::Invalid;  // Return Invalid if no match is found
			}

			const std::string_view& LocationBank::ToCountryStringView(ECountry country)
			{
				if (country == ECountry::Invalid)
				{
					assert(false && "Invalid country");
				}
				return m_countryListLookUp[static_cast<size_t>(country) - 1];  // Offset to match correct index
			}

			EDistrict LocationBank::FromDistrictString(const std::string_view& districtName)
			{
				for (size_t i = 0; i < Districts; ++i)
				{
					if (m_districtListLookUp[i] == districtName)
					{
						return static_cast<EDistrict>(i + 1);  // Offset by 1 to skip EDistrict::Invalid
					}
				}
				return EDistrict::Invalid;  // Return Invalid if no match is found
			}

			const std::string_view& LocationBank::ToDistrictStringView(EDistrict district)
			{
				if (district == EDistrict::Invalid)
				{
					assert(false && "Invalid district");
				}
				return m_districtListLookUp[static_cast<size_t>(district) - 1];  // Offset to match correct index
			}

			ECity LocationBank::FromCityString(const std::string_view& cityName)
			{
				for (size_t i = 0; i < Countries; ++i)
				{
					for (size_t j = 0; j < CitiesPerCountry; ++j)
					{
						if (m_countryCityListLookUp[i][j] == cityName)
						{
							return static_cast<ECity>(i * CitiesPerCountry + j + 1);  // Offset by 1 to skip ECity::Invalid
						}
					}
				}
				return ECity::Invalid;  // Return Invalid if no match is found
			}

			const std::string_view& LocationBank::ToCityStringView(ECity city)
			{
				if (city == ECity::Invalid)
				{
					assert(false && "Invalid city");
				}
				size_t index = static_cast<size_t>(city) - 1;  // Offset to match correct index
				size_t countryIndex = index / CitiesPerCountry;
				size_t cityIndex = index % CitiesPerCountry;
				return m_countryCityListLookUp[countryIndex][cityIndex];
			}

		}  // namespace data
	}  // namespace analysis
}  // namespace paplease
