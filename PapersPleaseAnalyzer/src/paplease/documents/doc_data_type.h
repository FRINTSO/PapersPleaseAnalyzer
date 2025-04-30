#pragma once
#include "paplease/types.h"

namespace paplease {
    namespace documents {

        enum class FieldCategory : u8
        {
            Invalid = static_cast<u8>(-1),
            CountryList = 0,            // Ex. REPUBLIA, ARSTOTZKA, OBRISTAN, ANTEGRIA
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

            Min = CountryList,
            Max = TranscriptPage,
            Count = Max - Min + 1
        };

        // Enumeration to specify the type of data being handled
        enum class DataType : u8
        {
            Invalid = static_cast<u8>(-1),
            GenericString = 0,
            GenericNumber,
            StrList,
            Date,
            Image,
            SIUnit,
            Vaccine,
            Sex,
        };

        // Enumeration to define the type of field within a document
        enum class FieldType : u8
        {
            Invalid = static_cast<u8>(-1),
            Text = 0,
            Image,
        };

        // Enumeration to represent the state of a data field
        enum class FieldState : u8
        {
            Empty = static_cast<u8>(-1),
            Initialized = 0,
            ProcessedData,
            BrokenData,
        };

    }  // namespace documents
}  // namspace paplease
