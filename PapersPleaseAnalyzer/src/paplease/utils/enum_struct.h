#pragma once

#include <type_traits>

namespace paplease {
    namespace utils {

		template <typename T>
		class EnumBase
		{
			static_assert(std::is_integral_v<T>, "T must be an integral type");

		public:
			constexpr EnumBase() noexcept : m_data() {}
			constexpr EnumBase(T value) noexcept : m_data(value) {}
			constexpr operator T() const noexcept { return m_data; }

		private:
			T m_data;
		};


    }
}