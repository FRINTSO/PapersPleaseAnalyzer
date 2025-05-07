#pragma once
#include <array>
#include <bitset>
#include <compare>
#include <iterator>
#include <type_traits>
#include <xutility>

namespace paplease {
    namespace core {
        
        template<class T, size_t Size>
        class FixedArrayConstIterator
        {
        public:
            using iterator_concept  = std::contiguous_iterator_tag;
            using iterator_category = std::random_access_iterator_tag;
            using value_type        = T;
            using difference_type   = std::ptrdiff_t;
            using pointer           = const T*;
            using reference         = const T&;

            constexpr FixedArrayConstIterator() noexcept : m_ptr() {}

            constexpr explicit FixedArrayConstIterator(pointer parg, size_t offset = 0) noexcept : m_ptr(parg + offset) {}

            [[nodiscard]] constexpr reference operator*() const { return *m_ptr; }

            [[nodiscard]] constexpr pointer operator->() const { return m_ptr; }

            constexpr FixedArrayConstIterator& operator++() noexcept
            {
                ++m_ptr;
                return *this;
            }

            constexpr FixedArrayConstIterator operator++(int) noexcept
            {
                FixedArrayConstIterator tmp = *this;
                ++*this;
                return tmp;
            }

            constexpr FixedArrayConstIterator& operator--() noexcept
            {
                --m_ptr;
                return *this;
            }

            constexpr FixedArrayConstIterator operator--(int) noexcept
            {
                FixedArrayConstIterator tmp = *this;
                --*this;
                return tmp;
            }

            constexpr FixedArrayConstIterator& operator+=(const std::ptrdiff_t offset) noexcept
            {
                m_ptr += offset;
                return *this;
            }

            constexpr FixedArrayConstIterator& operator-=(const std::ptrdiff_t offset) noexcept
            {
                m_ptr -= offset;
                return *this;
            }

            [[nodiscard]] constexpr std::ptrdiff_t operator-(const FixedArrayConstIterator& right) const noexcept
            {
                return m_ptr - right.m_ptr;
            }

            [[nodiscard]] constexpr reference operator[](const std::ptrdiff_t offset) const noexcept
            {
                return m_ptr[offset];
            }

            [[nodiscard]] constexpr bool operator==(const FixedArrayConstIterator& right) const noexcept
            {
                return m_ptr == right.m_ptr;
            }

            [[nodiscard]] constexpr std::strong_ordering operator<=>(const FixedArrayConstIterator& right) const noexcept
            {
                return m_ptr <=> right.m_ptr;
            }

        private:
            pointer m_ptr; // pointer to element in vector

        public:
            [[nodiscard]] constexpr FixedArrayConstIterator operator+(const ptrdiff_t offset) const noexcept
            {
                FixedArrayConstIterator tmp = *this;
                tmp += offset;
                return tmp;
            }

            [[nodiscard]] constexpr FixedArrayConstIterator operator-(const ptrdiff_t offset) const noexcept
            {
                FixedArrayConstIterator tmp = *this;
                tmp -= offset;
                return tmp;
            }

            [[nodiscard]] friend constexpr FixedArrayConstIterator operator+(
                const ptrdiff_t offset, FixedArrayConstIterator next) noexcept
            {
                next += offset;
                return next;
            }
        };

        template<class T, size_t Size>
        class FixedArrayIterator : public FixedArrayConstIterator<T, Size>
        {
        public:
            using MyBase = FixedArrayConstIterator<T, Size>;

            using iterator_concept  = std::contiguous_iterator_tag;
            using iterator_category = std::random_access_iterator_tag;
            using value_type        = T;
            using difference_type   = std::ptrdiff_t;
            using pointer           = T*;
            using reference         = T&;

            constexpr FixedArrayIterator() noexcept {}

            constexpr explicit FixedArrayIterator(pointer parg, size_t offset = 0) noexcept : MyBase(parg, offset) {}

            [[nodiscard]] constexpr reference operator*() const noexcept
            {
                return const_cast<reference>(MyBase::operator*());
            }

            [[nodiscard]] constexpr pointer operator->() const noexcept
            {
                return const_cast<pointer>(MyBase::operator->());
            }

            constexpr FixedArrayIterator& operator++() noexcept
            {
                MyBase::operator++();
                return *this;
            }

            constexpr FixedArrayIterator operator++(int) noexcept
            {
                FixedArrayIterator tmp = *this;
                MyBase::operator++();
                return tmp;
            }

            constexpr FixedArrayIterator& operator--() noexcept
            {
                MyBase::operator--();
                return *this;
            }

            constexpr FixedArrayIterator operator--(int) noexcept
            {
                FixedArrayIterator tmp = *this;
                MyBase::operator--();
                return tmp;
            }

            constexpr FixedArrayIterator& operator+=(const std::ptrdiff_t offset) noexcept
            {
                MyBase::operator+=(offset);
                return *this;
            }

            [[nodiscard]] constexpr FixedArrayIterator operator+(const std::ptrdiff_t offset) const noexcept
            {
                FixedArrayIterator tmp = *this;
                tmp += offset;
                return tmp;
            }

            [[nodiscard]] friend constexpr FixedArrayIterator operator+(const std::ptrdiff_t offset, FixedArrayIterator next) noexcept
            {
                next += offset;
                return next;
            }

            constexpr FixedArrayIterator& operator-=(const std::ptrdiff_t offset) noexcept
            {
                MyBase::operator-=(offset);
                return *this;
            }

            using MyBase::operator-;

            [[nodiscard]] constexpr FixedArrayIterator operator-(const std::ptrdiff_t offset) const noexcept
            {
                FixedArrayIterator tmp = *this;
                tmp -= offset;
                return tmp;
            }

            [[nodiscard]] constexpr reference operator[](const std::ptrdiff_t offset) const noexcept
            {
                return const_cast<reference>(MyBase::operator[](offset));
            }
        };

        // --- FixedArray<T, Size> ---
        template<typename T, size_t Size>
        class FixedArray
        {
        public:
            static_assert(std::is_object_v<T>, "The C++ Standard forbids containers of non-object types "
                                               "because of [container.requirements].");
            //static_assert(std::is_default_constructible_v<T>, "Object has to be default constructible.");

            using value_type      = T;
            using pointer         = value_type*;
            using const_pointer   = const value_type*;
            using reference       = value_type&;
            using const_reference = const value_type&;
            using size_type       = std::size_t;
            using difference_type = std::ptrdiff_t;

            using iterator               = FixedArrayIterator<value_type, Size>;
            using const_iterator         = FixedArrayConstIterator<value_type, Size>;
            using reverse_iterator       = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

            constexpr FixedArray() : m_data(), m_count(0) {}

            constexpr FixedArray(const size_type count, const T& value) : m_count(count)
            {
                assert(count <= Size);
                for (size_t i = 0; i < count; i++)
                {
                    m_data[i] = value;
                }
            }

            // template<class Iter, std::enable_if_t<std::_Is_iterator_v<Iter>, int> = 0>
            // constexpr FixedArray(Iter first, Iter last) { assert(false); }

            // constexpr FixedArray(std::initializer_list<T> initializer_list){ assert(false); }

            constexpr FixedArray(const FixedArray& right)
                : m_count(right.m_count)
            {
                assert(right.m_count <= Size);

                for (size_t i = 0; i < right.m_count; i++)
                {
                    m_data[i] = right.m_data[i];
                }
            }

            constexpr FixedArray(FixedArray&& right) noexcept
                : m_count(right.m_count)
            {
                for (size_t i = 0; i < right.m_count; i++)
                {
                    m_data[i] = std::move(right.m_data[i]);
                }

                right.m_count = 0;
            }

            constexpr FixedArray& operator=(FixedArray&& right) noexcept
            {
                if (this != &right)
                {
                    for (size_t i = 0; i < right.m_count; i++)
                    {
                        m_data[i] = std::move(right.m_data[i]);
                    }

                    // Move the count
                    m_count = right.m_count;

                    // Optionally leave the source in a valid state
                    right.m_count = 0;
                }
                return *this;
            }

            constexpr ~FixedArray(){
                m_count = 0;
            }

        public:
            constexpr FixedArray& operator=(const FixedArray& right)
            {
                if (this != &right)
                {
                    assert(right.m_count <= Size); // Ensure size consistency
                    for (size_t i = 0; i < right.m_count; i++)
                    {
                        m_data[i] = right.m_data[i];
                    }
                    m_count = right.m_count;
                }
                return *this;
            }

        public:
            template<class ValueType>
            constexpr reference EmplaceBack(ValueType&& value)
            {
                if (m_count >= Size)
                {
                    __debugbreak();
                }
                m_data[m_count] = std::forward<ValueType>(value);
                return m_data[m_count++];
            }

            constexpr void Add(const T& value)
            {
                EmplaceBack(value);
            }

            constexpr void Add(T&& value)
            {
                EmplaceBack(std::move(value));
            }

        public:
            constexpr void PopBack() {}

            constexpr iterator Erase(const_iterator where) {}
            
            constexpr iterator Erase(const_iterator first, const_iterator last) {}

            constexpr void Clear() noexcept
            {
                m_count = 0;
            }

            constexpr void Swap(FixedArray& right) {}

            [[nodiscard]] constexpr T* Data() noexcept
            {
                return m_data;
            }

            [[nodiscard]] constexpr const T* Data() const noexcept
            {
                return m_data;
            }

            [[nodiscard]] constexpr iterator begin() noexcept
            {
                return iterator(m_data, 0);
            }

            [[nodiscard]] constexpr const_iterator begin() const noexcept
            {
                return const_iterator(m_data, 0);
            }

            [[nodiscard]] constexpr iterator end() noexcept
            {
                return iterator(m_data, m_count);
            }

            [[nodiscard]] constexpr const_iterator end() const noexcept
            {
                return const_iterator(m_data, m_count);
            }

            [[nodiscard]] constexpr reverse_iterator rbegin() noexcept
            {
                return reverse_iterator(end());
            }

            [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
            {
                return const_reverse_iterator(end());
            }

            [[nodiscard]] constexpr reverse_iterator rend() noexcept
            {
                return reverse_iterator(begin());
            }

            [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
            {
                return const_reverse_iterator(begin());
            }

            [[nodiscard]] constexpr const_iterator cbegin() const noexcept
            {
                return begin();
            }

            [[nodiscard]] constexpr const_iterator cend() const noexcept
            {
                return end();
            }

            [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
            {
                return rbegin();
            }

            [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
            {
                return rend();
            }

            [[nodiscard]] constexpr bool Empty() const noexcept {
                return m_count == 0;
            }

            [[nodiscard]] constexpr bool Full() const noexcept
            {
                return m_count == Size;
            }

            [[nodiscard]] constexpr size_type Count() const noexcept
            {
                return m_count;
            }

            [[nodiscard]] constexpr size_type MaxSize() const noexcept {
                return Size;
            }

            [[nodiscard]] constexpr T& operator[](const size_type pos) noexcept
            {
                assert(pos < m_count && "FixedArray: Index out of bounds");
                return m_data[pos];
            }

            [[nodiscard]] constexpr const T& operator[](const size_type pos) const noexcept
            {
                assert(pos < m_count && "FixedArray: Index out of bounds");
                return m_data[pos];
            }

        private:
            T m_data[Size];
            size_t m_count;
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

            constexpr T& Get(Enum key) noexcept
            {
                return m_array[ToIndex(key)];
            }
            constexpr const T& Get(Enum key) const noexcept
            {
                return m_array[ToIndex(key)];
            }
            constexpr void Set(Enum key, const T& value) noexcept
            {
                size_t index = ToIndex(key);
                m_array[index] = value;
                m_set[index] = true;
            }
            constexpr void Set(Enum key, T&& value) noexcept
            {
                size_t index = ToIndex(key);
                m_array[index] = std::move(value);
                m_set[index] = true;
            }
            constexpr bool Contains(Enum key) const noexcept
            {
                return m_set[ToIndex(key)];
            }

            constexpr void Clear() noexcept
            {
                for (auto& entry : m_array)
                    entry = m_defaultValue;
                m_set.reset();
            }

            // Range support
            class Iterator
            {
            public:
                using iterator_category = std::forward_iterator_tag;
                using value_type        = T;
                using difference_type   = std::ptrdiff_t;
                using pointer           = T*;
                using reference         = T&;

                constexpr Iterator(T* ptr, T* end, const std::bitset<Size>* set)
                    : m_begin(ptr), m_ptr(ptr), m_end(end), m_set(set)
                {
                    SkipInvalid();
                }

                constexpr reference operator*() { return *m_ptr; }
                constexpr pointer operator->() const { return m_ptr; }

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
                const T* m_begin;
                T* m_ptr;
                const T* m_end;
                const std::bitset<Size>* m_set;

                constexpr void SkipInvalid()
                {
                    while (m_ptr != m_end && !m_set->test((size_t)(m_ptr - m_begin)))
                    {
                        ++m_ptr;
                    }
                }
            };
            
            constexpr auto begin() { return Iterator(m_array.data(), m_array.data() + Size, &m_set); }
            constexpr auto end() { return Iterator(m_array.data() + Size, m_array.data() + Size, &m_set); }

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
            // static_assert(std::is_integral_v<KeyType>);

            enum class EntryState : unsigned char { Empty, Occupied, Deleted };

            class Entry
            {
            public:
                KeyType key{};
                ValueType value{};
                constexpr const ValueType* operator->() const noexcept
                {
                    return &value;
                }

            private:
                EntryState state = EntryState::Empty;

                constexpr Entry() = default;

                friend class FixedHashTable;
            };

            constexpr size_t GetHashIndex(const KeyType& key) const
            {
                return Hash{}(key) % Size;
            }

        public:
            constexpr FixedHashTable() = default;

            //template <typename T = ValueType>
            //constexpr std::enable_if_t<!std::is_const_v<T>, bool>
            constexpr bool Get(const KeyType& key, ValueType*& outValue) noexcept
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
                    if (entry.state == EntryState::Deleted)
                    {
                        continue;
                    }
                    if (entry.key == key)
                    {
                        outValue = &entry.value;
                        return true;
                    }
                }
                return false;
            }

            //template <typename T = ValueType>
            //constexpr std::enable_if_t<std::is_const_v<T>, bool>
            constexpr bool Get(const KeyType& key, const ValueType*& outValue) const noexcept
            {
                const size_t start = GetHashIndex(key);
                for (size_t offset = 0; offset < Size; offset++)
                {
                    const size_t idx = (start + offset) % Size;
                    const Entry& entry = m_data[idx];

                    if (entry.state == EntryState::Empty)
                    {
                        break;
                    }
                    if (entry.state == EntryState::Deleted)
                    {
                        continue;
                    }
                    if (entry.key == key)
                    {
                        outValue = &entry.value;
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

                    if (entry.state == EntryState::Empty || entry.state == EntryState::Deleted)
                    {
                        entry.key = key;
                        entry.value = value;
                        entry.state = EntryState::Occupied;
                        m_count++;
                        return true;
                    }
                    else if (entry.key == key)
                    {
                        entry.value = value;
                        return true;
                    }
                }
                return false;
            }
            constexpr bool Set(const KeyType& key, ValueType&& value)
            {
                const size_t start = GetHashIndex(key);
                for (size_t offset = 0; offset < Size; offset++)
                {
                    const size_t idx = (start + offset) % Size;
                    Entry& entry = m_data[idx];

                    if (entry.state == EntryState::Empty || entry.state == EntryState::Deleted)
                    {
                        entry.key = key;
                        entry.value = std::move(value);
                        entry.state = EntryState::Occupied;
                        m_count++;
                        return true;
                    }
                    else if (entry.key == key)
                    {
                        entry.value = std::move(value);
                        return true;
                    }
                }
                return false;
            }

            constexpr bool Remove(const KeyType& key)
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
                    if (entry.state == EntryState::Deleted)
                    {
                        continue;
                    }
                    else if (entry.key == key)
                    {
                        // entry.value = std::move(value);
                        entry.state = EntryState::Deleted;
                        m_count--;
                        return true;
                    }
                }
                return false;
            }

            constexpr bool Contains(const KeyType& key) const noexcept
            {
                const ValueType* v = nullptr;
                return Get(key, v);
            }

            constexpr size_t Count() const noexcept
            {
                return m_count;
            }

            constexpr void Clear()
            {
                for (auto& entry : m_data)
                {
                    entry.state = EntryState::Empty;
                }
                m_count = 0;
            }

            constexpr ValueType& operator[](const KeyType& key)
            {
                const size_t start = GetHashIndex(key);
                for (size_t offset = 0; offset < Size; offset++)
                {
                    const size_t idx = (start + offset) % Size;
                    Entry& entry = m_data[idx];

                    if (entry.state == EntryState::Empty || entry.state == EntryState::Deleted)
                    {
                        break;
                    }
                    if (entry.key == key)
                    {
                        return entry.value;
                    }
                }
//                __debugbreak();
            }

            constexpr const ValueType& operator[](const KeyType& key) const
            {
                const size_t start = GetHashIndex(key);
                for (size_t offset = 0; offset < Size; offset++)
                {
                    const size_t idx = (start + offset) % Size;
                    const Entry& entry = m_data[idx];

                    if (entry.state == EntryState::Empty || entry.state == EntryState::Deleted)
                    {
                        break;
                    }
                    if (entry.key == key)
                    {
                        return entry.value;
                    }
                }
                assert(false);
            }

            // Iterator support
            class ConstIterator
            {
            public:
                using iterator_category = std::forward_iterator_tag;
                using value_type = Entry;
                using difference_type = std::ptrdiff_t;
                using pointer = const value_type*;
                using reference = const value_type&;

                constexpr ConstIterator(const Entry* ptr, const Entry* end)
                    : m_ptr(ptr), m_end(end)
                {
                    SkipInvalid();
                }

                constexpr reference operator*() const { return *m_ptr; }
                constexpr pointer operator->() const { return m_ptr; }

                constexpr ConstIterator& operator++()
                {
                    ++m_ptr;
                    SkipInvalid();
                    return *this;
                }

                constexpr ConstIterator operator++(int)
                {
                    ConstIterator temp = *this;
                    ++(*this);
                    return temp;
                }

                friend constexpr bool operator==(const ConstIterator& a, const ConstIterator& b)
                {
                    return a.m_ptr == b.m_ptr;
                }

                friend constexpr bool operator!=(const ConstIterator& a, const ConstIterator& b)
                {
                    return !(a == b);
                }

            private:
                const Entry* m_ptr;
                const Entry* m_end;

                constexpr void SkipInvalid()
                {
                    while (m_ptr != m_end && m_ptr->state != EntryState::Occupied)
                    {
                        ++m_ptr;
                    }
                }
            };

            class Iterator
            {
            public:
                using iterator_category = std::forward_iterator_tag;
                using value_type = Entry;
                using difference_type = std::ptrdiff_t;
                using pointer = Entry*;
                using reference = Entry&;

                constexpr Iterator(Entry* ptr, Entry* end)
                    : m_ptr(ptr), m_end(end)
                {
                    SkipInvalid();
                }

                constexpr reference operator*() { return *m_ptr; }
                constexpr pointer operator->() const { return m_ptr; }

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
            constexpr ConstIterator begin() const { return ConstIterator(m_data.data(), m_data.data() + Size); }
            constexpr ConstIterator end() const { return ConstIterator(m_data.data() + Size, m_data.data() + Size); }
            
        private:
            std::array<Entry, Size> m_data{};
            size_t m_count = 0;
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
            class ConstIterator
            {
            public:
                using iterator_category = std::forward_iterator_tag;
                using value_type        = T;
                using difference_type   = std::ptrdiff_t;
                using pointer           = const value_type*;
                using reference         = const value_type&;

                constexpr ConstIterator(const Bucket* ptr, const Bucket* end)
                    : m_ptr(ptr), m_end(end)
                {
                    SkipInvalid();
                }

                constexpr reference operator*() const { return m_ptr->value; }
                constexpr pointer operator->() const { return &m_ptr->value; }

                constexpr ConstIterator& operator++()
                {
                    ++m_ptr;
                    SkipInvalid();
                    return *this;
                }

                constexpr ConstIterator operator++(int)
                {
                    ConstIterator temp = *this;
                    ++(*this);
                    return temp;
                }

                friend constexpr bool operator==(const ConstIterator& a, const ConstIterator& b)
                {
                    return a.m_ptr == b.m_ptr;
                }

                friend constexpr bool operator!=(const ConstIterator& a, const ConstIterator& b)
                {
                    return !(a == b);
                }

            private:
                const Bucket* m_ptr;
                const Bucket* m_end;

                constexpr void SkipInvalid()
                {
                    while (m_ptr != m_end && m_ptr->state != EntryState::Occupied)
                    {
                        ++m_ptr;
                    }
                }
            };

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
            constexpr ConstIterator begin() const { return ConstIterator(m_buckets.data(), m_buckets.data() + Size); }
            constexpr ConstIterator end() const { return ConstIterator(m_buckets.data() + Size, m_buckets.data() + Size); }
        };

    }  // namespace core
} // namespace paplease
