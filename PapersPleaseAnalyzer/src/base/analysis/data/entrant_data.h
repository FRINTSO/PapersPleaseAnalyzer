#pragma once

#include "base/analysis/data/location_bank.h"

namespace paplease {
    namespace analysis {
        namespace data {

            enum class EEntryPurpose : unsigned int
            {
                Invalid = -1,
                Work,
                Diplomacy,
                Asylum,
                Count,
            };

            struct EntrantInfo
            {
                bool IsCitizen() const;

                ECountry nationality = ECountry::Invalid;
                EDistrict district = EDistrict::Invalid;
                ECity city = ECity::Invalid;

                EEntryPurpose purpose;
            };

        }
    }
}