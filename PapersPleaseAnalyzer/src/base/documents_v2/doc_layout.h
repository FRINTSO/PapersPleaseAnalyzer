#pragma once
#include <array>

#include "base/documents_v2/doc_data_type.h"
#include "base/documents_v2/doc_type.h"
#include "base/shape.h"

namespace paplease {
	namespace documents {
		namespace v2 {

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
					constexpr DataLayout(Rectangle dataBox, DataFieldCategory dataFieldCategory, FieldType dataFieldType) noexcept;

					[[nodiscard]] constexpr const Rectangle GetBox() const noexcept;
					[[nodiscard]] constexpr const DataFieldCategory GetCategory() const noexcept;
					[[nodiscard]] constexpr const FieldType GetType() const noexcept;
				private:
					const Rectangle m_dataBox;
					const DataFieldCategory m_dataFieldCategory;
					const FieldType m_dataFieldType;
				};

			public:
				static constexpr size_t MaxLayouts = 10;
			public:
				constexpr Rectangle GetFieldBox(DataFieldCategory type);
				constexpr const std::array<Rectangle, DocLayout::MaxLayouts> GetAllFieldBoxes();

				constexpr const DataLayout GetLayout(DataFieldCategory type) const noexcept;
				constexpr const DataLayout* GetAllLayouts() const noexcept;
				constexpr const size_t GetLayoutCount() const noexcept;
			private:
				constexpr DocLayout() noexcept;
				constexpr DocLayout(std::array<DataLayout, DocLayout::MaxLayouts> layouts, size_t layoutCount) noexcept;

			private:
				const std::array<DataLayout, DocLayout::MaxLayouts> m_layouts;
				const size_t m_layoutCount;
			};

		}  // namespace v2
	}  // namespase documents
}  // namespace paplease

#include "base/documents_v2/doc_layout.inl.h"