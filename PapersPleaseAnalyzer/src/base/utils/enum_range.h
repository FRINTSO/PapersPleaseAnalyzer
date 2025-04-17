#pragma once

#include <type_traits>

namespace paplease {
    namespace utils {

        // Helper function to convert enum values to their underlying integer type
        template <typename E>
        constexpr auto to_underlying(E e) noexcept
        {
            return static_cast<std::underlying_type_t<E>>(e);
        }

        // Iterator for enum ranges
        template <typename E>
        class EnumIterator
        {
            int value;
        public:
            explicit EnumIterator(int v) : value(v) {}

            E operator*() const { return static_cast<E>(value); }

            EnumIterator& operator++()
            {
                ++value;
                return *this;
            }

            bool operator!=(const EnumIterator& other) const
            {
                return value != other.value;
            }
        };

        // EnumRange class that stores the range and provides begin/end iterators
        template <typename E>
        class EnumRange
        {
            int begin_value, end_value;
        public:
            EnumRange(E begin, E end)
                : begin_value(to_underlying(begin)), end_value(to_underlying(end))
            {}

            EnumIterator<E> begin() const
            {
                return EnumIterator<E>(begin_value);
            }

            EnumIterator<E> end() const
            {
                return EnumIterator<E>(end_value + 1);
            }
        };

        // Function to create an EnumRange from two enum values
        template <typename E>
        EnumRange<E> enum_range(E begin, E end)
        {
            return EnumRange<E>(begin, end);
        }

    }  // namespace utils
}  // namespace paplease
