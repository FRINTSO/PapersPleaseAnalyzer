#pragma once
#include <array>

#include "paplease/common/color.h"
#include "paplease/documents/doc_type.h"
#include "paplease/common/shape.h"

namespace paplease {
	namespace documents {

		class DocAppearance
		{
		public:
			[[nodiscard]] static constexpr const DocAppearance Get(AppearanceType type) noexcept;
			[[nodiscard]] static const DocAppearance& GetRef(AppearanceType type) noexcept;
			[[nodiscard]] static consteval const DocAppearance GetInstant(AppearanceType type) noexcept;
		public:
			constexpr DocAppearance() noexcept;

			[[nodiscard]] constexpr const RgbColor* GetColors() const noexcept;
			[[nodiscard]] constexpr size_t GetColorCount() const noexcept;
			[[nodiscard]] constexpr const Shape GetShape() const noexcept;
			[[nodiscard]] constexpr const int GetWidth() const noexcept;
			[[nodiscard]] constexpr const int GetHeight() const noexcept;
			[[nodiscard]] constexpr const AppearanceType GetType() const noexcept;
		private:
			static constexpr size_t MaxBorderColors = 4;
		private:
			constexpr DocAppearance(const std::array<RgbColor, MaxBorderColors>& borderColors, const size_t colorCount, const ScaledShape shape, const AppearanceType appearanceType) noexcept;
		private:
			const std::array<RgbColor, MaxBorderColors> m_borderColors;
			const size_t m_colorCount;
			const Shape m_shape;
			const AppearanceType m_appearanceType;
		};

	}  // namespace documents
}  // namespace paplease


#include "paplease/documents/doc_appearance.inl.h"