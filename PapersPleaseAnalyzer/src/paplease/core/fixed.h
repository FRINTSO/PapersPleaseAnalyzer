#pragma once
#include <array>
#include <bitset>
#include <iterator>
#include <type_traits>

namespace paplease {
    namespace core {

        // --- FixedArray<T, Size> ---
        template<typename T, size_t Size>
        class FixedArray
        {
        public:
            constexpr FixedArray(const T& defaultValue = T()) : m_array{}, m_count{}, m_default{ defaultValue }
            {
                Clear();
            }

            constexpr void Add(const T& value)
            {
                //assert(m_count < Size && "FixedArray: Add out of bounds");
                if (m_count >= Size)
                {
                    __debugbreak();
                }
                m_array[m_count++] = value;
            }

            constexpr void Add(T&& value)
            {
                //assert(m_count < Size && "FixedArray: Add out of bounds");
                if (m_count >= Size)
                {
                    __debugbreak();
                }
                m_array[m_count++] = std::move(value);
            }

            constexpr T& EmplaceBack(T&& value)
            {
                // assert(m_count < Size && "FixedArray: Add out of bounds");
                if (m_count >= Size)
                {
                    __debugbreak();
                }
                m_array[m_count] = std::move(value);
                return m_array[m_count++];
            }

            constexpr const T& operator[](size_t index) const
            {
                assert(index < m_count && "FixedArray: Index out of bounds");
                return m_array[index];
            }

            constexpr T& operator[](size_t index)
            {
                assert(index < m_count && "FixedArray: Index out of bounds");
                return m_array[index];
            }

            constexpr size_t Count() const { return m_count; }
            constexpr size_t Capacity() const { return Size; }
            constexpr bool Empty() const { return m_count == 0; }
            constexpr bool Full() const { return m_count == Size; }

            constexpr void Clear()
            {
                for (auto& entry : m_array)
                    entry = m_default;
                m_count = 0;
            }

            // Range support
            constexpr auto begin() { return m_array.begin(); }
            constexpr auto end() { return m_array.begin() + m_count; }

            constexpr auto begin() const { return m_array.begin(); }
            constexpr auto end() const { return m_array.begin() + m_count; }

        private:
            std::array<T, Size> m_array;
            size_t m_count;
            T m_default;
        };

        // --- FixedTable<Enum, T> ---
        template<typename Enum, typename T>
        class FixedTable
        {
            static_assert(std::is_enum_v<Enum>, "Enum must be an enum type.");
            static_assert(static_cast<std::underlying_type_t<Enum>>(Enum::Count) > 0,
                          "KeyEnum must define Count as the last entry.");
            static_assert(std::is_default_constructible_v<T>,
                          "Value type T must be default constructible.");
            static_assert(std::is_copy_constructible_v<T> || std::is_move_constructible_v<T>,
                          "Value type T must be either copy-constructible or move-constructible.");
            static_assert(std::is_integral_v<std::underlying_type_t<Enum>>,
                          "Enum's underlying type must be integral.");

            static inline constexpr size_t Size = static_cast<size_t>(Enum::Count);

        public:
            explicit constexpr FixedTable(const T& defaultValue = T())
                : m_array(), m_set(), m_defaultValue(defaultValue)
            {
                Clear();
            }

            constexpr T& Get(Enum key)
            {
                return m_array[ToIndex(key)];
            }
            constexpr const T& Get(Enum key) const
            {
                return m_array[ToIndex(key)];
            }
            constexpr void Set(Enum key, const T& value)
            {
                size_t index = ToIndex(key);
                m_array[index] = value;
                m_set[index] = true;
            }
            constexpr void Set(Enum key, T&& value)
            {
                size_t index = ToIndex(key);
                m_array[index] = std::move(value);
                m_set[index] = true;
            }
            constexpr bool Contains(Enum key) const
            {
                return m_set[ToIndex(key)];
            }

            constexpr void Clear()
            {
                for (auto& entry : m_array)
                    entry = m_defaultValue;
                m_set.reset();
            }

            // Range support
            constexpr auto begin() { return m_array.begin(); }
            constexpr auto end() { return m_array.begin() + Size; }

            constexpr auto begin() const { return m_array.begin(); }
            constexpr auto end() const { return m_array.begin() + Size; }

        private:
            constexpr size_t ToIndex(Enum key) const
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

        // --- FixedHashTable<KeyType, ValueType, Size> ---
        template<typename KeyType, typename ValueType, size_t Size, typename Hash = std::hash<KeyType>>
        class FixedHashTable
        {
        private:
            static_assert(std::is_integral_v<KeyType>);

            enum class EntryState : unsigned char { Empty, Occupied, Deleted };

            class Entry
            {
            public:
                KeyType key{};
                ValueType value{};
            public:
                EntryState state = EntryState::Empty;

                constexpr Entry() = default;
            };

            constexpr size_t GetHashIndex(const KeyType& key)
            {
                return Hash{}(key) % Size;
                // return (size_t)key % Size;
            }

        public:
            constexpr FixedHashTable() = default;

            constexpr bool Get(const KeyType& key, ValueType& outValue)
            {
                const size_t start = GetHashIndex(key);
                for (size_t offset = 0; offset < Size; offset++)
                {
                    const size_t idx = (start + offset) % Size;
                    Entry& entry = m_data[idx];

                    if (entry.state == EntryState::Empty)
                    {
                        break;
                    }
                    if (entry.key == key)
                    {
                        outValue = entry.value;
                        return true;
                    }
                }
                return false;
            }

            constexpr bool Set(const KeyType& key, const ValueType& value)
            {
                const size_t start = GetHashIndex(key);
                for (size_t offset = 0; offset < Size; offset++)
                {
                    const size_t idx = (start + offset) % Size;
                    Entry& entry = m_data[idx];

                    if (entry.state == EntryState::Empty)
                    {
                        entry.key = key;
                        entry.value = value;
                        entry.state = EntryState::Occupied;
                        return true;
                    }
                    else if (entry.key == key)
                    {
                        entry.value = value;
                        return true;
                    }
                }
                // assert(false && "Hash table is full, cannot insert new key-value pair.");
                return false;
            }
            constexpr void Set(const KeyType& key, ValueType&& value)
            {
                const size_t start = GetHashIndex(key);
                for (size_t offset = 0; offset < Size; offset++)
                {
                    const size_t idx = (start + offset) % Size;
                    Entry& entry = m_data[idx];

                    if (entry.state == EntryState::Empty)
                    {
                        entry.key = key;
                        entry.value = std::move(value);
                        entry.state = EntryState::Occupied;
                        return;
                    }
                    else if (entry.key == key)
                    {
                        entry.value = std::move(value);
                        return;
                    }
                }

                //assert(false && "Hash table is full, cannot insert new key-value pair.");
            }

            constexpr bool Contains(const KeyType& key)
            {
                ValueType v;
                return Get(key, v);
            }

            // Iterator support
            /*class Iterator
            {
            public:
                using iterator_category = std::forward_iterator_tag;
                using value_type = EntryState;
                using difference_type = std::ptrdiff_t;
                using pointer = EntryState*;
                using reference = EntryState&;

                constexpr Iterator(Entry* ptr, Entry* end)
                    : m_ptr(ptr), m_end(end)
                {
                    SkipInvalid();
                }

                constexpr reference operator*() { return *m_ptr; }
                constexpr pointer operator->() const { return &m_ptr; }

                constexpr Iterator& operator++()
                {
                    ++m_ptr;
                    SkipInvalid();
                    return *this;
                }

                constexpr Iterator operator++(int)
                {
                    Iterator temp = *this;
                    ++(*this);
                    return temp;
                }

                friend constexpr bool operator==(const Iterator& a, const Iterator& b)
                {
                    return a.m_ptr == b.m_ptr;
                }

                friend constexpr bool operator!=(const Iterator& a, const Iterator& b)
                {
                    return !(a == b);
                }

            private:
                Entry* m_ptr;
                Entry* m_end;

                constexpr void SkipInvalid()
                {
                    while (m_ptr != m_end && m_ptr->state != EntryState::Occupied)
                    {
                        ++m_ptr;
                    }
                }
            };

            constexpr Iterator begin() { return Iterator(m_data.data(), m_data.data() + Size); }
            constexpr Iterator end() { return Iterator(m_data.data() + Size, m_data.data() + Size); }
            */
        private:
            std::array<Entry, Size> m_data{};
        };

        // --- FixedHashSet<Enum, T> ---
        template<typename T, size_t Size, typename Hash = std::hash<T>>
        class FixedHashSet
        {
        private:
            enum class EntryState : unsigned char { Empty, Occupied, Deleted };

            struct Bucket
            {
                T value{};
                EntryState state = EntryState::Empty;

                constexpr Bucket() = default;
            };

            std::array<Bucket, Size> m_buckets{};
            size_t m_count = 0;

            constexpr size_t GetHashIndex(const T& key) const
            {
                return Hash{}(key) % Size;
            }

        public:
            constexpr FixedHashSet() = default;

            constexpr bool Insert(const T& value)
            {
                const size_t start = GetHashIndex(value);
                for (size_t offset = 0; offset < Size; offset++)
                {
                    const size_t idx = (start + offset) % Size;
                    Bucket& bucket = m_buckets[idx];

                    if (bucket.state != EntryState::Occupied)
                    {
                        bucket.value = value;
                        bucket.state = EntryState::Occupied;
                        ++m_count;
                        return true;
                    }
                    if (bucket.value == value)
                    {
                        return false;
                    }
                }
                return false;
            }

            constexpr bool Remove(const T& value)
            {
                const size_t start = GetHashIndex(value);
                for (size_t offset = 0; offset < Size; offset++)
                {
                    const size_t idx = (start + offset) % Size;
                    Bucket& bucket = m_buckets[idx];

                    if (bucket.state == EntryState::Occupied && bucket.value == value)
                    {
                        bucket.state == EntryState::Deleted;
                        --m_count;
                        return true;
                    }
                    if (bucket.state == EntryState::Empty)
                    {
                        break;
                    }
                }
                return false;
            }

            constexpr bool Contains(const T& value) const
            {
                const size_t start = GetHashIndex(value);
                for (size_t offset = 0; offset < Size; offset++)
                {
                    const size_t idx = (start + offset) % Size;
                    const Bucket& bucket = m_buckets[idx];

                    if (bucket.state == EntryState::Occupied && bucket.value == value)
                    {
                        return true;
                    }
                    if (bucket.state == EntryState::Empty)
                    {
                        break;
                    }
                }
                return false;
            }
            
            constexpr void Clear()
            {
                for (auto& bucket : m_buckets)
                {
                    bucket.state = EntryState::Empty;
                }
                m_count = 0;
            }

            constexpr size_t Count() const
            {
                return m_count;
            }

            bool Empty() const
            {
                return m_count == 0;
            }

            // Iterator support
            class Iterator
            {
            public:
                using iterator_category = std::forward_iterator_tag;
                using value_type = T;
                using difference_type = std::ptrdiff_t;
                using pointer = T*;
                using reference = T&;

                constexpr Iterator(Bucket* ptr, Bucket* end)
                    : m_ptr(ptr), m_end(end)
                {
                    SkipInvalid();
                }

                constexpr reference operator*() const { return m_ptr->value; }
                constexpr pointer operator->() const { return &m_ptr->value; }

                constexpr Iterator& operator++()
                {
                    ++m_ptr;
                    SkipInvalid();
                    return *this;
                }

                constexpr Iterator operator++(int)
                {
                    Iterator temp = *this;
                    ++(*this);
                    return temp;
                }

                friend constexpr bool operator==(const Iterator& a, const Iterator& b)
                {
                    return a.m_ptr == b.m_ptr;
                }

                friend constexpr bool operator!=(const Iterator& a, const Iterator& b)
                {
                    return !(a == b);
                }

            private:
                Bucket* m_ptr;
                Bucket* m_end;

                constexpr void SkipInvalid()
                {
                    while (m_ptr != m_end && m_ptr->state != EntryState::Occupied)
                    {
                        ++m_ptr;
                    }
                }
            };

            constexpr Iterator begin() { return Iterator(m_buckets.data(), m_buckets.data() + Size); }
            constexpr Iterator end() { return Iterator(m_buckets.data() + Size, m_buckets.data() + Size); }
        };
    }  // namespace core
} // namespace paplease
