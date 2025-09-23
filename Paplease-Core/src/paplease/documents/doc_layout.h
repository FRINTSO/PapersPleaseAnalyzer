#pragma once
#include "paplease/common/shape.h"
#include "paplease/core/fixed.h"
#include "paplease/documents/doc_data_type.h"
#include "paplease/documents/doc_type.h"

#include <array>

namespace paplease {
	namespace documents {

		class DocLayout
		{
		public:
			[[nodiscard]] static constexpr const DocLayout Get(AppearanceType type) noexcept;
			[[nodiscard]] static const DocLayout& GetRef(AppearanceType type) noexcept;
			[[nodiscard]] static consteval const DocLayout GetInstant(AppearanceType type) noexcept;

			[[nodiscard]] static consteval const DocLayout GetBooth() noexcept;
		public:
			class DataLayout
			{
			public:
				constexpr DataLayout() noexcept;
				constexpr DataLayout(ScaledRectangle dataBox, FieldCategory dataFieldCategory, FieldType dataFieldType) noexcept;

				[[nodiscard]] constexpr const Rectangle GetBox() const noexcept;
				[[nodiscard]] constexpr const FieldCategory GetCategory() const noexcept;
				[[nodiscard]] constexpr const FieldType GetType() const noexcept;
			private:
				const Rectangle m_dataBox;
				const FieldCategory m_dataFieldCategory;
				const FieldType m_dataFieldType;
			};

		public:
			static constexpr size_t MaxLayouts = 10;
		public:
			constexpr Rectangle GetFieldBox(FieldCategory type);
			constexpr const std::array<Rectangle, DocLayout::MaxLayouts> GetAllFieldBoxes();

			constexpr const DataLayout& GetLayout(FieldCategory type) const noexcept;
			constexpr const DataLayout* GetAllLayouts() const noexcept;
			constexpr const size_t GetLayoutCount() const noexcept;

			constexpr const core::FixedArray<const DataLayout*, MaxLayouts> GetLayouts() const noexcept;
		private:
			constexpr DocLayout() noexcept;
			constexpr DocLayout(std::array<DataLayout, DocLayout::MaxLayouts> layouts, size_t layoutCount) noexcept;

		private:
			const std::array<DataLayout, DocLayout::MaxLayouts> m_layouts;
			const size_t m_layoutCount;
		};

	}  // namespase documents
}  // namespace paplease

#include "paplease/documents/doc_layout.inl.h"