#pragma once
#include "paplease/analysis/data/location_bank.h"
#include "paplease/analysis/data/rules.h"

namespace paplease {
    namespace analysis {
        namespace data {

            class EntrantClass
            {
            public:
                enum : u8
                {
                    Entrant       = 0b00'00'00'00,   // All instances of this is an entrant

                    // Entrant specialization, must be one of these
                    Citizen       = 0b00'00'00'01,   // bitflag
                    Foreigner     = 0b00'00'00'10,   // bitflag

                    // Foreigner specialization, must be one of these
                    Worker        = 0b00'00'01'10,
                    Diplomat      = 0b00'00'10'10,
                    AsylumSeeker  = 0b00'00'11'10,

                    // Other, must be one of these
                    FromKolechia  = 0b00'01'00'10,
                    FromImpor     = 0b00'10'00'10,
                    FromUnitedFed = 0b00'11'00'10,

                    // Citizen specialization
                    FromAltanDistrict = 0b01'00'00'01,
                };

                static inline constexpr u8 CountryMask = 0b00'00'00'11;
                static inline constexpr size_t Count = 10;
                
                constexpr bool IsCitizen() const noexcept
                {
                    return (m_data & Citizen) == Citizen;
                }
                constexpr bool IsForeigner() const noexcept
                {
                    return (m_data & Foreigner) == Foreigner;
                }

                constexpr bool IsWorker() const noexcept
                {
                    return m_data == Worker;
                }
                constexpr bool IsDiplomat() const noexcept
                {
                    return m_data == Diplomat;
                }
                constexpr bool IsAsylumSeeker() const noexcept
                {
                    return m_data == AsylumSeeker;
                }

                constexpr bool IsFromKolechia() const noexcept
                {
                    return (m_data & FromKolechia) == FromKolechia;
                }
                constexpr bool IsFromImpor() const noexcept
                {
                    return (m_data & FromImpor) == FromImpor;
                }
                constexpr bool IsFromUnitedFed() const noexcept
                {
                    return (m_data & FromUnitedFed) == FromUnitedFed;
                }

                constexpr bool IsFromAltanDistrict() const noexcept
                {
                    return (m_data & FromAltanDistrict) == FromAltanDistrict;
                }

                constexpr bool IsTarget(ERuleTarget ruleTarget) const noexcept
                {
                    switch (ruleTarget)
                    {
                        case ERuleTarget::Invalid:
                            return false;
                        case ERuleTarget::Entrant:
                            return true;
                        case ERuleTarget::Citizens:
                            return IsCitizen();
                        case ERuleTarget::Foreigners:
                            return IsForeigner();
                        case ERuleTarget::Workers:
                            return IsWorker();
                        case ERuleTarget::Diplomats:
                            return IsDiplomat();
                        case ERuleTarget::AsylumSeekers:
                            return IsAsylumSeeker();
                        case ERuleTarget::Kolechians:
                            return IsFromKolechia();
                        case ERuleTarget::FromImpor:
                            return IsFromImpor();
                        case ERuleTarget::FromUnitedFederation:
                            return IsFromUnitedFed();
                        case ERuleTarget::FromAltanDistrict:
                            return IsFromAltanDistrict();
                    }
                }

                constexpr EntrantClass(u8 value) noexcept : m_data(value) {}
                constexpr explicit operator u8() const noexcept { return m_data; }

                // Addition operator
                constexpr EntrantClass operator+(const EntrantClass& other) const noexcept
                {
                    return EntrantClass(m_data + other.m_data);
                }
                constexpr EntrantClass& operator+=(const EntrantClass& other)
                {
                    m_data += other.m_data;
                    return *this;
                }

                // Subtraction operator
                constexpr EntrantClass operator-(const EntrantClass& other) const noexcept
                {
                    return EntrantClass(m_data - other.m_data);
                }
                constexpr EntrantClass& operator-=(const EntrantClass& other)
                {
                    m_data -= other.m_data;
                    return *this;
                }

                // Multiplication operator
                constexpr EntrantClass operator*(const EntrantClass& other) const noexcept
                {
                    return EntrantClass(m_data * other.m_data);
                }
                constexpr EntrantClass& operator*=(const EntrantClass& other)
                {
                    m_data *= other.m_data;
                    return *this;
                }

                // Division operator
                constexpr EntrantClass operator/(const EntrantClass& other) const noexcept
                {
                    return EntrantClass(m_data / other.m_data);
                }
                constexpr EntrantClass& operator/=(const EntrantClass& other)
                {
                    m_data /= other.m_data;
                    return *this;
                }

                // Equality operator
                constexpr bool operator==(const EntrantClass& other) const noexcept
                {
                    return m_data == other.m_data;
                }

                // Inequality operator
                constexpr bool operator!=(const EntrantClass& other) const noexcept
                {
                    return m_data != other.m_data;
                }

                // Bitwise AND operator
                constexpr EntrantClass operator&(const EntrantClass& other) const noexcept
                {
                    return EntrantClass(m_data & other.m_data);
                }
                constexpr EntrantClass& operator&=(const EntrantClass& other)
                {
                    m_data &= other.m_data;
                    return *this;
                }

                // Bitwise OR operator
                constexpr EntrantClass operator|(const EntrantClass& other) const noexcept
                {
                    return EntrantClass(m_data | other.m_data);
                }
                constexpr EntrantClass& operator|=(const EntrantClass& other)
                {
                    m_data |= other.m_data;
                    return *this;
                }

                // Bitwise XOR operator
                constexpr EntrantClass operator^(const EntrantClass& other) const noexcept
                {
                    return EntrantClass(m_data ^ other.m_data);
                }
                constexpr EntrantClass& operator^=(const EntrantClass& other)
                {
                    m_data ^= other.m_data;
                    return *this;
                }

                // Bitwise NOT operator
                constexpr EntrantClass operator~() const noexcept
                {
                    return EntrantClass(~m_data);
                }

                // Bitwise left shift operator
                constexpr EntrantClass operator<<(int shift) const noexcept
                {
                    return EntrantClass(m_data << shift);
                }
                constexpr EntrantClass& operator<<=(int shift)
                {
                    m_data <<= shift;
                    return *this;
                }

                // Bitwise right shift operator
                constexpr EntrantClass operator>>(int shift) const noexcept
                {
                    return EntrantClass(m_data >> shift);
                }
                constexpr EntrantClass& operator>>=(int shift)
                {
                    m_data >>= shift;
                    return *this;
                }

            private:
                u8 m_data;
            };

            struct EntrantInfo
            {
                EntrantClass entrantClass = EntrantClass::Entrant;

                ECountry nationality = ECountry::Invalid;
                EDistrict district = EDistrict::Invalid;
                ECity city = ECity::Invalid;

                void SetNationaility(ECountry country)
                {
                    if ((entrantClass & entrantClass.CountryMask) != 0)
                    {
                        LOG_WARN("Setting country twice");
                    }
                    else
                    {
                        if (country == ECountry::Arstotzka)
                        {
                            entrantClass |= data::EntrantClass::Citizen;
                            LOG("Is citizen");
                        }
                        else
                        {
                            entrantClass |= data::EntrantClass::Foreigner;
                            LOG("Is foreigner");
                        }
                    }
                    
                    nationality = country;
                }
                void SetEntrantDistrict(EDistrict district)
                {
                    this->district = district;
                }
                void SetEntrantCity(ECity city)
                {
                    this->city = city;
                }

            };

        }
    }
}