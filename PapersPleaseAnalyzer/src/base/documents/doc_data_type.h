#pragma once
#include "base/common.h"

namespace paplease {
    namespace documents {

        enum class DataFieldCategory
        {
            Invalid = 0,
            // Nationality,            // Ex. OBRISTAN
            CountryList,            // Ex. REPUBLIA, ARSTOTZKA, OBRISTAN, ANTEGRIA
            // Description,            // Ex. 
            District,               // Ex. BURNTON
            // Duration,               //
            DurationOfStay,         // Ex. 3 MONTHS; FOREVER; 2 DAYS
            Field,                  // Refers to job position: FISHING; CONSTRUCTION
            FingerPrints,           // <PHOTO>
            IssuingCity,            // Ex. Lesrenadi; West Grestin
            IssuingCountry,         // Ex. KOLECHIA
            Purpose,                // Ex. VISIT; WORK; TRANSIT; IMMIGRATE
            PhysicalAppearance,     // Ex. WIDOW'S PEAK; SHORT STRAIGHT HAIR
            
            // Are these used?      //
            //HasValidSeal,         //
            //PassportType,         //
            
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
                //case DataFieldCategory::Nationality:
                //    return "Nationality";
                case DataFieldCategory::CountryList:
                    return "CountryList";
                case DataFieldCategory::DateOfBirth:
                    return "DateOfBirth";
                //case DataFieldCategory::Description:
                //    return "Description";
                case DataFieldCategory::District:
                    return "District";
                //case DataFieldCategory::Duration:
                //    return "Duration";
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

    }  // namespace documents
}  // namspace paplease
