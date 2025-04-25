#pragma once
#include "paplease/common/common.h"

namespace paplease {

    template<typename T = int>
    class scaled
    {
    public:
        static_assert(std::is_integral<T>::value, "scaled_int only accepts integral types");

        consteval scaled() = default;
        consteval scaled(T value)
            : m_value{
#if IS_DOWNSCALED
                DOWNSCALE(value)
#else
                value
#endif
            }
        {}

        constexpr operator T() const { return m_value; }

    private:
        T m_value;
    };

}  // namespace paplease
