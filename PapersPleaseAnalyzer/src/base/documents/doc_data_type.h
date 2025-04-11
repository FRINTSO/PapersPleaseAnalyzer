#pragma once
#include "base/common.h"

namespace paplease {
    namespace documents {

        enum class FieldCategory : unsigned int
        {
            Invalid = -1,
            CountryList,            // Ex. REPUBLIA, ARSTOTZKA, OBRISTAN, ANTEGRIA
            District,               // Ex. BURNTON
            DurationOfStay,         // Ex. 3 MONTHS; FOREVER; 2 DAYS
            Field,                  // Refers to job position: FISHING; CONSTRUCTION
            FingerPrints,           // <PHOTO>
            IssuingCity,            // Ex. Lesrenadi; West Grestin
            IssuingCountry,         // Ex. KOLECHIA
            Purpose,                // Ex. VISIT; WORK; TRANSIT; IMMIGRATE
            PhysicalAppearance,     // Ex. WIDOW'S PEAK; SHORT STRAIGHT HAIR
            
            ValidDate,              // Ex. 25.11.1982
            EndDate,                // Ex. 01.05.1983
            ExpirationDate,         // Ex. 20.12.1982

            DateOfBirth,            // Ex. 06.02.1953
            Name,                   // Ex. STEPHANI HODGOVSKA
            PassportNumber,         // Ex. D79CA-XTGXX
            Sex,                    // Ex. F

            Photo,                  // <PHOTO>
            ThumbPrint,             // <PHOTO>

            Vaccination1,           // Ex. 15.12.81  TUBERC.
            Vaccination2,           // Ex. 17.12.81  POLIO
            Vaccination3,           // Ex. <EMPTY>

            Height,                 // Ex. 176cm
            Weight,                 // Ex. 97kg

            BoothDate,              // Ex. 03.01.83
            BoothCounter,           // Ex. 01

            Rule1, Rule2, Rule3, Rule4, Rule5, Rule6, Rule7, Rule8, Rule9, Rule10,
            CriminalPhoto1, CriminalPhoto2, CriminalPhoto3,
            TranscriptPage,

            Count
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
        enum class FieldState
        {
            Empty = 0,
            Initialized,
            ProcessedData
        };

#if USE_ENUM_FUNCS
        constexpr const std::string FieldCategoryAsString(FieldCategory fieldCategory)
        {
            switch (fieldCategory)
            {
                case FieldCategory::Invalid:
                    return "Invalid";
                case FieldCategory::CountryList:
                    return "CountryList";
                case FieldCategory::DateOfBirth:
                    return "DateOfBirth";
                case FieldCategory::District:
                    return "District";
                case FieldCategory::DurationOfStay:
                    return "DurationOfStay";
                case FieldCategory::EndDate:
                    return "EndDate";
                case FieldCategory::ExpirationDate:
                    return "ExpirationDate";
                case FieldCategory::Field:
                    return "Field";
                case FieldCategory::FingerPrints:
                    return "FingerPrints";
                case FieldCategory::Height:
                    return "Height";
                case FieldCategory::IssuingCity:
                    return "IssuingCity";
                case FieldCategory::IssuingCountry:
                    return "IssuingCountry";
                case FieldCategory::Name:
                    return "Name";
                case FieldCategory::PassportNumber:
                    return "PassportNumber";
                case FieldCategory::Photo:
                    return "Photo";
                case FieldCategory::PhysicalAppearance:
                    return "PhysicalAppearance";
                case FieldCategory::Purpose:
                    return "Purpose";
                case FieldCategory::Sex:
                    return "Sex";
                case FieldCategory::ThumbPrint:
                    return "ThumbPrint";
                case FieldCategory::Vaccination1:
                    return "Vaccination1";
                case FieldCategory::Vaccination2:
                    return "Vaccination2";
                case FieldCategory::Vaccination3:
                    return "Vaccination3";
                case FieldCategory::ValidDate:
                    return "ValidDate";
                case FieldCategory::Weight:
                    return "Weight";
                default:
                    return "";
            }
        }
#endif

    }  // namespace documents
}  // namspace paplease
