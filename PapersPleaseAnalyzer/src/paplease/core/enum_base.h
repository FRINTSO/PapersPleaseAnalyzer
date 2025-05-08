#pragma once
#include <type_traits>

namespace paplease {
    namespace core {

        template<typename T>
        class EnumBase
        {
            static_assert(std::is_integral_v<T>, "T must be an integral type.");
        public:
            constexpr EnumBase() = default;
            constexpr EnumBase(T value) noexcept : m_data(value) {}
            constexpr operator T() const noexcept { return static_cast<T>(m_data); }

            // operators
            //constexpr auto& operator+=(const EnumBase other) noexcept
            //{
            //    m_data += other.m_data;
            //    return *this;
            //}

            //constexpr auto& operator-=(const EnumBase other) noexcept
            //{
            //    m_data -= other.m_data;
            //    return *this;
            //}

            //constexpr auto& operator*=(const EnumBase other) noexcept
            //{
            //    m_data *= other.m_data;
            //    return *this;
            //}

            //constexpr auto& operator/=(const EnumBase other) noexcept
            //{
            //    m_data /= other.m_data;
            //    return *this;
            //}

            constexpr auto& operator&=(const EnumBase other) noexcept
            {
                m_data &= other.m_data;
                return *this;
            }

            constexpr auto& operator|=(const EnumBase other) noexcept
            {
                m_data |= other.m_data;
                return *this;
            }

            constexpr auto& operator^=(const EnumBase other) noexcept
            {
                m_data ^= other.m_data;
                return *this;
            }

            constexpr auto& operator<<=(const EnumBase other) noexcept
            {
                m_data <<= other.m_data;
                return *this;
            }

            constexpr auto& operator>>=(const EnumBase other) noexcept
            {
                m_data >>= other.m_data;
                return *this;
            }

            constexpr bool HasFlag(const EnumBase other) const noexcept
            {
                return (m_data & other) == other;
            }

            constexpr bool HasAnyFlag(const EnumBase other) const noexcept
            {
                return (m_data & other) != 0;
            }

        protected:
            T m_data;
        };

    }
}