#pragma once
#include <array>

#include "base/common.h"
#include "base/documents_v2/doc_appearance.h"
#include "base/documents_v2/doc_data.h"
#include "base/shape.h"


namespace Documents::V2
{

class DocLayout {
public:
	[[nodiscard]] static constexpr const DocLayout Get(AppearanceType type) noexcept;

public:
	class DataLayout {
	public:
		constexpr DataLayout() noexcept;
		constexpr DataLayout(Rectangle dataBox, DataFieldCategory dataFieldCategory, DataFieldType dataFieldType) noexcept;

		[[nodiscard]] constexpr const Rectangle GetBox() const noexcept;
		[[nodiscard]] constexpr const DataFieldCategory GetCategory() const noexcept;
		[[nodiscard]] constexpr const DataFieldType GetType() const noexcept;
	private:
		const Rectangle m_dataBox;
		const DataFieldCategory m_dataFieldCategory;
		const DataFieldType m_dataFieldType;
	};

public:
	constexpr Rectangle GetFieldBox(DataFieldCategory type);
	constexpr const std::array<Rectangle, 9> GetAllFieldBoxes();

	constexpr const DataLayout GetLayout(DataFieldCategory type) const noexcept;
	constexpr const DataLayout* GetAllLayouts() const noexcept;
	constexpr const size_t GetLayoutCount() const noexcept;
private:
	constexpr DocLayout() noexcept;
	constexpr DocLayout(std::array<DataLayout, 9> layouts, size_t layoutCount, AppearanceType type) noexcept;

	constexpr size_t CountValidLayouts(std::array<DataLayout, 9> layouts) const noexcept;

private:
	const std::array<DataLayout, 9> m_layouts;
	const size_t m_layoutCount;
	const AppearanceType m_appearanceType;
};

} // namespace Documents::V2

#include "base/documents_v2/doc_layout.inl.h"