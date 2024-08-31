#pragma once
#include <array>

#include "base/common.h"
#include "base/documents_v2/doc_appearance.h"
#include "base/documents_v2/doc_data.h"
#include "base/shape.h"


namespace Documents::V2 {

	class DocLayout
	{
	public:
		static constexpr size_t Layouts = 10;

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
		constexpr Rectangle GetFieldBox(DataFieldCategory type);
		constexpr const std::array<Rectangle, DocLayout::Layouts> GetAllFieldBoxes();

		constexpr const DataLayout GetLayout(DataFieldCategory type) const noexcept;
		constexpr const DataLayout* GetAllLayouts() const noexcept;
		constexpr const size_t GetLayoutCount() const noexcept;
	private:
		constexpr DocLayout() noexcept;
		constexpr DocLayout(std::array<DataLayout, DocLayout::Layouts> layouts, size_t layoutCount) noexcept;

	private:
		const std::array<DataLayout, DocLayout::Layouts> m_layouts;
		const size_t m_layoutCount;
	};

} // namespace Documents::V2

#include "base/documents_v2/doc_layout.inl.h"