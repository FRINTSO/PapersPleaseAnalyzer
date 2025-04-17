#pragma once
#include <type_traits>

#include "base/utils/metaprogramming.h"

namespace paplease {
	namespace documents {
        namespace detail {
            static consteval bool is_integer_type(FieldCategory category)
            {
                return category == FieldCategory::BoothCounter;
            }

            static consteval bool is_string_type(FieldCategory category)
            {
                switch (category)
                {
                    case FieldCategory::Field:
                    case FieldCategory::IssuingCity:
                    case FieldCategory::PhysicalAppearance:
                    case FieldCategory::District:
                    case FieldCategory::IssuingCountry:
                    case FieldCategory::Name:
                    case FieldCategory::DurationOfStay:
                    case FieldCategory::Purpose:
                    case FieldCategory::PassportNumber:
                        return true;
                    default:
                        return false;
                }
            }

            static consteval bool is_date_type(FieldCategory category)
            {
                switch (category)
                {
                    case FieldCategory::ValidDate:
                    case FieldCategory::EndDate:
                    case FieldCategory::ExpirationDate:
                    case FieldCategory::DateOfBirth:
                    case FieldCategory::BoothDate:
                        return true;
                    default:
                        return false;
                }
            }

            static consteval bool is_siunit_type(FieldCategory category)
            {
                switch (category)
                {
                    case FieldCategory::Height:
                    case FieldCategory::Weight:
                        return true;
                    default:
                        return false;
                }
            }

            static consteval bool is_vaccine_type(FieldCategory category)
            {
                switch (category)
                {
                    case FieldCategory::Vaccination1:
                    case FieldCategory::Vaccination2:
                    case FieldCategory::Vaccination3:
                        return true;
                    default:
                        return false;
                }
            }

            static consteval bool is_strlist_type(FieldCategory category)
            {
                return category == FieldCategory::CountryList;
            }

            static consteval bool is_sex_type(FieldCategory category)
            {
                return category == FieldCategory::Sex;
            }

            static consteval bool is_photo_type(FieldCategory category)
            {
                return category == FieldCategory::Photo;
            }

        } // namespace detail

        template<FieldCategory Category>
        struct detail::field_data_type<Category, std::enable_if_t<detail::is_integer_type(Category)>> : utils::has_type<int>{};

        template<FieldCategory Category>
        struct detail::field_data_type<Category, std::enable_if_t<detail::is_string_type(Category)>> : utils::has_type<std::string>{};

        template<FieldCategory Category>
        struct detail::field_data_type<Category, std::enable_if_t<detail::is_date_type(Category)>> : utils::has_type<data::Date>{};

        template<FieldCategory Category>
        struct detail::field_data_type<Category, std::enable_if_t<detail::is_siunit_type(Category)>> : utils::has_type<data::SIUnitValue>{};

        template<FieldCategory Category>
        struct detail::field_data_type<Category, std::enable_if_t<detail::is_vaccine_type(Category)>> : utils::has_type<data::Vaccine>{};

        template<FieldCategory Category>
        struct detail::field_data_type<Category, std::enable_if_t<detail::is_strlist_type(Category)>> : utils::has_type<data::StrList>{};

        template<FieldCategory Category>
        struct detail::field_data_type<Category, std::enable_if_t<detail::is_sex_type(Category)>> : utils::has_type<data::Sex>{};

        template<FieldCategory Category>
        struct detail::field_data_type<Category, std::enable_if_t<detail::is_photo_type(Category)>> : utils::has_type<data::Photo>{};

        template<FieldCategory Category>
        constexpr detail::FieldDataType<Category> Field::GetFieldData() const
        {
            if (this->Type() == FieldType::Invalid)
            {
                return std::nullopt;
            }

            const auto& innerData = this->GetData();
            return innerData.Get<detail::field_data_type_t<Category>>();
        }

        template<FieldCategory Category>
        constexpr detail::FieldDataType<Category> DocData::GetFieldData() const
        {
            auto fieldDataOpt = this->GetField(Category);
            if (!fieldDataOpt)
                return std::nullopt;

            const auto& fieldData = fieldDataOpt.value().get();

            if (fieldData.Type() == FieldType::Invalid || fieldData.IsBroken())
            {
                return std::nullopt;
            }

            const auto& innerData = fieldData.GetData();
            return innerData.Get<detail::field_data_type_t<Category>>();
        }

	}  // namespace documents
}  // namespace paplease
