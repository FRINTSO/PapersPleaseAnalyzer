#pragma once
#include "base/common.h"

namespace paplease {
    namespace documents {
        namespace v2 {

            enum class DataFieldCategory
            {
                Invalid = 0,
                Nationality,
                CountryList,
                Description,
                District,
                Duration,
                DurationOfStay,
                Field, // Refers to job position
                FingerPrints,
                IssuingCity,
                IssuingCountry,
                Purpose,
                PhysicalAppearance,

                // Are these used?
                //HasValidSeal,
                //PassportType,

                ValidDate,
                EndDate,
                ExpirationDate,

                DateOfBirth,
                Name,
                PassportNumber,
                Sex,

                Photo,
                ThumbPrint,

                Vaccination1,
                Vaccination2,
                Vaccination3,

                Height,
                Weight,

                BoothDate,
                BoothCounter,

                Rule1, Rule2, Rule3, Rule4, Rule5, Rule6, Rule7, Rule8, Rule9, Rule10,
                CriminalPhoto1, CriminalPhoto2, CriminalPhoto3,
                TranscriptPage,

                DATA_FIELD_CATEGORY_LENGTH
            };

            // Enumeration to specify the type of data being handled
            enum class DataType
            {
                Invalid = 0,
                GenericString,
                GenericNumber,
                StrList,
                Date,
                Image,
                SIUnit,
                Vaccine,
                Sex
            };

            // Enumeration to define the type of field within a document
            enum class FieldType
            {
                Invalid = 0,
                Text,
                Image
            };

            // Enumeration to represent the state of a data field
            enum class DataFieldState
            {
                Empty = 0,
                Initialized,
                ProcessedData
            };

#if USE_ENUM_FUNCS
            constexpr const std::string DataFieldCategoryAsString(DataFieldCategory dataFieldCategory)
            {
                switch (dataFieldCategory)
                {
                    case DataFieldCategory::Invalid:
                        return "Invalid";
                    case DataFieldCategory::Nationality:
                        return "Nationality";
                    case DataFieldCategory::CountryList:
                        return "CountryList";
                    case DataFieldCategory::DateOfBirth:
                        return "DateOfBirth";
                    case DataFieldCategory::Description:
                        return "Description";
                    case DataFieldCategory::District:
                        return "District";
                    case DataFieldCategory::Duration:
                        return "Duration";
                    case DataFieldCategory::DurationOfStay:
                        return "DurationOfStay";
                    case DataFieldCategory::EndDate:
                        return "EndDate";
                    case DataFieldCategory::ExpirationDate:
                        return "ExpirationDate";
                    case DataFieldCategory::Field:
                        return "Field";
                    case DataFieldCategory::FingerPrints:
                        return "FingerPrints";
                    case DataFieldCategory::Height:
                        return "Height";
                    case DataFieldCategory::IssuingCity:
                        return "IssuingCity";
                    case DataFieldCategory::IssuingCountry:
                        return "IssuingCountry";
                    case DataFieldCategory::Name:
                        return "Name";
                    case DataFieldCategory::PassportNumber:
                        return "PassportNumber";
                    case DataFieldCategory::Photo:
                        return "Photo";
                    case DataFieldCategory::PhysicalAppearance:
                        return "PhysicalAppearance";
                    case DataFieldCategory::Purpose:
                        return "Purpose";
                    case DataFieldCategory::Sex:
                        return "Sex";
                    case DataFieldCategory::ThumbPrint:
                        return "ThumbPrint";
                    case DataFieldCategory::Vaccination1:
                        return "Vaccination1";
                    case DataFieldCategory::Vaccination2:
                        return "Vaccination2";
                    case DataFieldCategory::Vaccination3:
                        return "Vaccination3";
                    case DataFieldCategory::ValidDate:
                        return "ValidDate";
                    case DataFieldCategory::Weight:
                        return "Weight";
                    default:
                        return "";
                }
            }
#endif

        }  // v2
    }  // namespace documents
}  // namspace paplease
