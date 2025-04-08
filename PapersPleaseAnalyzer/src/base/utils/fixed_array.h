#pragma once

#include <array>
#include <optional>
#include <type_traits>

namespace paplease {
	namespace utils {

		template<typename T, size_t Size>
		class FixedArray
		{
		public:
			FixedArray() : m_count(0) {}

			void Add(const T& value)
			{
				assert(m_count < Size && "FixedArray: Add out of bounds");
				m_array[m_count++] = value;
			}

			void Add(T&& value)
			{
				assert(m_count < Size && "FixedArray: Add out of bounds");
				m_array[m_count++] = std::move(value);
			}

			const T& operator[](size_t index) const
			{
				assert(index < m_count && "FixedArray: Index out of bounds");
				return m_array[index];
			}

			T& operator[](size_t index)
			{
				assert(index < m_count && "FixedArray: Index out of bounds");
				return m_array[index];
			}

			size_t Count() const { return m_count; }
			size_t Capacity() const { return Size; }
			bool Empty() const { return m_count == 0; }
			bool Full() const { return m_count == Size; }

			void Clear()
			{
				m_count = 0;
			}

			// Range support
			auto begin() { return m_array.begin(); }
			auto end() { return m_array.begin() + m_count; }

			auto begin() const { return m_array.begin(); }
			auto end() const { return m_array.begin() + m_count; }

		private:
			std::array<T, Size> m_array;
			size_t m_count;
		};

		template<typename T, size_t Size>
		using FixedOptionalArray = FixedArray<std::optional<T>, Size>;

		template<typename T, size_t Size>
		using FixedRefArray = FixedArray<std::optional<std::reference_wrapper<const T>>, Size>;

		template<typename T, size_t Size>
		using FixedPtrArray = FixedArray<const T*, Size>;

	}  // namespace utils
}  // namespace paplease
