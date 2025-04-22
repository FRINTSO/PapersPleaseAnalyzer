#pragma once
#include <array>
#include <bitset>
#include <type_traits>

namespace paplease {
    namespace utils {

        template<typename Enum, typename T>
        class FixedTable
        {
            static_assert(std::is_enum_v<Enum>);
            static_assert(static_cast<std::underlying_type_t<Enum>>(Enum::Count) > 0, "KeyEnum must define Count as the last entry.");

            static constexpr size_t Size = static_cast<size_t>(Enum::Count);
        public:
            explicit FixedTable(const T& defaultValue)
                : m_array{}, m_set{}, m_defaultValue{defaultValue}
            {
                Clear();
            }

            T& Get(Enum key)
            {
                return m_array[ToIndex(key)];
            }
            const T& Get(Enum key) const
            {
                return m_array[ToIndex(key)];
            }
            void Set(Enum key, const T& value)
            {
                size_t index = ToIndex(key);
                m_array[index] = value;
                m_set[index] = true;
            }
            bool Has(Enum key) const
            {
                return m_set[ToIndex(key)];
            }

            void Clear()
            {
                for (auto& entry : m_array)
                    entry = m_defaultValue;
                m_set.reset();
            }

            // Range support
            auto begin() { return m_array.begin(); }
            auto end() { return m_array.begin() + Size; }

            auto begin() const { return m_array.begin(); }
            auto end() const { return m_array.begin() + Size; }

        private:
            size_t ToIndex(Enum key) const
            {
                size_t index = static_cast<size_t>(key);
                assert(index < static_cast<size_t>(Enum::Count) && "Key was outside of the enum!");
                return index;
            }
        private:
            std::array<T, Size> m_array{};
            std::bitset<Size> m_set{};  // Tracks which keys were Set
            T m_defaultValue;
        };

    }  // namespace utils
}  // namespace paplease
