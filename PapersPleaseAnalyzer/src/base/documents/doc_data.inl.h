#pragma once

#include "base/documents/data/date.h"

#include <type_traits>

namespace paplease {
	namespace documents {
        namespace details {
            static consteval bool is_integer_type(DataFieldCategory category)
            {
                return category == DataFieldCategory::BoothCounter;
            }

            static consteval bool is_string_type(DataFieldCategory category)
            {
                switch (category)
                {
                    case DataFieldCategory::Field:
                    case DataFieldCategory::IssuingCity:
                    case DataFieldCategory::PhysicalAppearance:
                    case DataFieldCategory::District:
                    case DataFieldCategory::IssuingCountry:
                    //case DataFieldCategory::Nationality:
                    case DataFieldCategory::DurationOfStay:
                    case DataFieldCategory::Purpose:
                    case DataFieldCategory::PassportNumber:
                        return true;
                    default:
                        return false;
                }
            }

            static consteval bool is_date_type(DataFieldCategory category)
            {
                switch (category)
                {
                    case DataFieldCategory::ValidDate:
                    case DataFieldCategory::EndDate:
                    case DataFieldCategory::ExpirationDate:
                    case DataFieldCategory::DateOfBirth:
                    case DataFieldCategory::BoothDate:
                        return true;
                    default:
                        return false;
                }
            }

            static consteval bool is_siunit_type(DataFieldCategory category)
            {
                switch (category)
                {
                    case DataFieldCategory::Height:
                    case DataFieldCategory::Weight:
                        return true;
                    default:
                        return false;
                }
            }

            static consteval bool is_vaccine_type(DataFieldCategory category)
            {
                switch (category)
                {
                    case DataFieldCategory::Vaccination1:
                    case DataFieldCategory::Vaccination2:
                    case DataFieldCategory::Vaccination3:
                        return true;
                    default:
                        return false;
                }
            }

            static consteval bool is_strlist_type(DataFieldCategory category)
            {
                return category == DataFieldCategory::CountryList;
            }

            static consteval bool is_sex_type(DataFieldCategory category)
            {
                return category == DataFieldCategory::Sex;
            }

        } // namespace details

        template<DataFieldCategory Category>
        struct DocData::data_field_type<Category, typename std::enable_if<details::is_integer_type(Category)>::type>
        {
            using type = int;
        };

        template<DataFieldCategory Category>
        struct DocData::data_field_type<Category, typename std::enable_if<details::is_string_type(Category)>::type>
        {
            using type = std::string;
        };

        template<DataFieldCategory Category>
        struct DocData::data_field_type<Category, typename std::enable_if<details::is_date_type(Category)>::type>
        {
            using type = data::Date;
        };

        template<DataFieldCategory Category>
        struct DocData::data_field_type<Category, typename std::enable_if<details::is_siunit_type(Category)>::type>
        {
            using type = data::SIUnitValue;
        };

        template<DataFieldCategory Category>
        struct DocData::data_field_type<Category, typename std::enable_if<details::is_vaccine_type(Category)>::type>
        {
            using type = data::Vaccine;
        };

        template<DataFieldCategory Category>
        struct DocData::data_field_type<Category, typename std::enable_if<details::is_strlist_type(Category)>::type>
        {
            using type = data::StrList;
        };

        template<DataFieldCategory Category>
        struct DocData::data_field_type<Category, typename std::enable_if<details::is_sex_type(Category)>::type>
        {
            using type = data::Sex;
        };


        template<DataFieldCategory Category>
        constexpr std::optional<std::reference_wrapper<const typename DocData::data_field_type<Category>::type>> DocData::GetFieldData() const
        {
            const auto& fieldData = this->GetField(Category);

            if (fieldData.Type() == FieldType::Invalid)
            {
                return std::nullopt;
            }

            const auto& innerData = fieldData.GetData();
            return innerData.Get<typename DocData::data_field_type<Category>::type>();
        }

	}  // namespace documents
}  // namespace paplease
