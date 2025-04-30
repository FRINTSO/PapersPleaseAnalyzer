#pragma once
#include "paplease/common/common.h"

namespace paplease {

    constexpr float SCALE = 0.5f;

    constexpr int Scale(int x)
    {
        if constexpr (IS_DOWNSCALED)
        {
            return static_cast<int>(x * SCALE);
        }
        else
        {
            return x;
        }
    }

    template<typename T = int>
    class scaled
    {
    public:
        static_assert(std::is_integral<T>::value, "scaled_int only accepts integral types");

        consteval scaled() = default;
        consteval scaled(T value)
            : m_value{
#if IS_DOWNSCALED
                Scale(value)
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
