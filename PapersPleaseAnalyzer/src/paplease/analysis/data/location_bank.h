#pragma once

#include <array>
#include <cassert>
#include <string_view>

#include "paplease/documents/doc_type.h"

namespace paplease {
    namespace analysis {
        namespace data {

			enum class ECountry : unsigned char
			{
				Invalid = -1,
				Antegria = 0,
				Arstotzka,
				Impor,
				Kolechia,
				Obristan,
				Republia,
				UnitedFederation,
				Count
			};

			enum class EDistrict : unsigned char
			{
				Invalid = -1,
				Altan = 0,
				Vescillo,
				Burnton,
				Octovalis,
				Gennistora,
				Lendiforma,
				Wozenfield,
				Fardesto,
				Count
			};

			enum class ECity : unsigned char
			{
				Invalid = -1,
				StMarmero = 0,
				Glorian,
				OuterGrouse,
				OrvechVonor,
				EastGrestin,
				Paradizna,
				Enkyo,
				Haihan,
				Tsunkeido,
				YurkoCity,
				Vedor,
				WestGrestin,
				Skal,
				Lorndaz,
				Mergerous,
				TrueGlorian,
				Lesrenadi,
				Bostan,
				GreatRapid,
				Shingleton,
				KoristaCity,
				Count
			};

			class LocationBank
			{
			public:
				static constexpr size_t Countries = 7;
				static constexpr size_t Districts = 8;
				static constexpr size_t CitiesPerCountry = 3;

				using CountryList = std::array<std::string_view, Countries>;
				using DistrictList = std::array<std::string_view, Districts>;
				using CityList = std::array<std::string_view, CitiesPerCountry>;
				using CountryCityLookup = std::array<CityList, Countries>;

				static constexpr const std::string_view& GetCountry(documents::PassportType passportType) noexcept
				{
					assert(passportType != documents::PassportType::Invalid && "PassportType cannot be invalid.");

					return m_countryListLookUp[static_cast<size_t>(passportType) - static_cast<int>(documents::DocType::Passport)];
				}
				static constexpr const CityList& GetCityList(documents::PassportType passportType) noexcept
				{
					assert(passportType != documents::PassportType::Invalid && "PassportType cannot be invalid.");

					return m_countryCityListLookUp[static_cast<size_t>(passportType) - static_cast<int>(documents::DocType::Passport)];
				}
				static bool IsValidCountry(const std::string_view& countryName);
				static bool IsValidCountry(const std::string_view& countryName, documents::PassportType passportType);
				static bool IsValidDistrict(const std::string_view& districtName);
				static bool IsValidCity(const std::string_view& cityName);
				static bool IsValidCity(const std::string_view& cityName, documents::PassportType passportType);

				static constexpr ECountry GetCountryFromPassportType(documents::PassportType passportType) noexcept
				{
					assert(passportType != documents::PassportType::Invalid && "PassportType cannot be invalid.");

					return static_cast<ECountry>(static_cast<size_t>(passportType) - static_cast<int>(documents::DocType::Passport));
				}

				static ECountry FromCountryString(const std::string_view& countryName);
				static const std::string_view& ToCountryStringView(ECountry country);
				static EDistrict FromDistrictString(const std::string_view& districtName);
				static const std::string_view& ToDistrictStringView(EDistrict district);
				static ECity FromCityString(const std::string_view& cityName);
				static const std::string_view& ToCityStringView(ECity city);

			private:
				LocationBank() = delete;

				static constexpr CountryList m_countryListLookUp = {
					"Antegria",
					"Arstotzka",
					"Impor",
					"Kolechia",
					"Obristan",
					"Republia",
					"United Federation",
				};

				static constexpr DistrictList m_districtListLookUp = {
					"Altan",
					"Vescillo",
					"Burnton",
					"Octovalis",
					"Gennistora",
					"Lendiforma",
					"Wozenfield",
					"Fardesto",
				};

				static constexpr CountryCityLookup m_countryCityListLookUp = { {
					{ "St. Marmero", "Glorian", "Outer Grouse" },         // Antegria
					{ "Orvech Vonor", "East Grestin", "Paradizna" },      // Arstotzka
					{ "Enkyo", "Haihan", "Tsunkeido" },                   // Impor
					{ "Yurko City", "Vedor", "West Grestin" },            // Kolechia
					{ "Skal", "Lorndaz", "Mergerous" },                   // Obristan
					{ "True Glorian", "Lesrenadi", "Bostan" },            // Republia
					{ "Great Rapid", "Shingleton", "Korista City" },      // United Federation
				} };
			};


        }  // namespace data
    }  // namespace analysis
}  // namespace paplease
