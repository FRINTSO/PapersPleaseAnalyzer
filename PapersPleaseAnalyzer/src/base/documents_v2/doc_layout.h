#pragma once
#include <array>

#include "base/common.h"
#include "base/documents_v2/doc_appearance.h"
#include "base/documents_v2/doc_data.h"
#include "base/shape.h"


namespace Documents::V2
{

#define CONSTEXPR constexpr

class DocLayout {
public:
	[[nodiscard]] static CONSTEXPR const DocLayout Get(AppearanceType type) noexcept;

public:
	class DataLayout {
	public:
		CONSTEXPR DataLayout() noexcept
			: m_dataBox{}, m_dataFieldCategory{}, m_dataFieldType{}
		{}
		CONSTEXPR DataLayout(Rectangle dataBox, DataFieldCategory dataFieldCategory, DataFieldType dataFieldType) noexcept
			: m_dataBox{ dataBox }, m_dataFieldCategory{ dataFieldCategory }, m_dataFieldType{ dataFieldType }
		{}

		[[nodiscard]] CONSTEXPR const Rectangle GetBox() const noexcept {
			return m_dataBox;
		}
		[[nodiscard]] CONSTEXPR const DataFieldCategory GetCategory() const noexcept {
			return m_dataFieldCategory;
		}
		[[nodiscard]] CONSTEXPR const DataFieldType GetType() const noexcept {
			return m_dataFieldType;
		}
	private:
		const Rectangle m_dataBox;
		const DataFieldCategory m_dataFieldCategory;
		const DataFieldType m_dataFieldType;
	};

public:
	CONSTEXPR Rectangle GetFieldBox(DataFieldCategory type);
	CONSTEXPR const std::array<Rectangle, 9> GetAllFieldBoxes();

	CONSTEXPR const DataLayout GetLayout(DataFieldCategory type) const noexcept;
	CONSTEXPR const DataLayout* GetAllLayouts() const noexcept;
	CONSTEXPR const size_t GetLayoutCount() const noexcept;
private:
	CONSTEXPR DocLayout() noexcept;
	CONSTEXPR DocLayout(std::array<DataLayout, 9> layouts, size_t layoutCount, AppearanceType type) noexcept;

	CONSTEXPR size_t CountValidLayouts(std::array<DataLayout, 9> layouts) const noexcept;

private:
	const std::array<DataLayout, 9> m_layouts;
	const size_t m_layoutCount;
	const AppearanceType m_appearanceType;
};

///////////////////////////// construction //////////
CONSTEXPR DocLayout::DocLayout() noexcept
	: m_layouts{}, m_layoutCount{}, m_appearanceType{}
{}

CONSTEXPR DocLayout::DocLayout(std::array<DataLayout, 9> layouts, size_t layoutCount, AppearanceType type) noexcept
	: m_layouts{ layouts }, m_layoutCount{ layoutCount }, m_appearanceType{ type }
{}

CONSTEXPR size_t DocLayout::CountValidLayouts(std::array<DataLayout, 9> layouts) const noexcept {
	size_t count = 0;
	for (size_t i = 0; i < 9; i++) {
		count += layouts[i].GetCategory() != DataFieldCategory::Invalid;
	}
	return count;
}

///////////////////////////// static methods //////////

CONSTEXPR const DocLayout DocLayout::Get(AppearanceType type) noexcept {
	switch (type)
	{

	case AppearanceType::AccessPermit:
		return DocLayout{
			{
				DataLayout{
					Rectangle{ DOWNSCALE(30), DOWNSCALE(108), DOWNSCALE(234), DOWNSCALE(16) },
					DataFieldCategory::Name,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(30), DOWNSCALE(152), DOWNSCALE(112), DOWNSCALE(16) },
					DataFieldCategory::Nationality,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(156), DOWNSCALE(152), DOWNSCALE(112), DOWNSCALE(16) },
					DataFieldCategory::PassportNumber,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(30), DOWNSCALE(196), DOWNSCALE(112), DOWNSCALE(16) },
					DataFieldCategory::Purpose,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(156), DOWNSCALE(196), DOWNSCALE(112), DOWNSCALE(16) },
					DataFieldCategory::DurationOfStay,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(30), DOWNSCALE(240), DOWNSCALE(112), DOWNSCALE(16) },
					DataFieldCategory::Height,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(156), DOWNSCALE(240), DOWNSCALE(112), DOWNSCALE(16) },
					DataFieldCategory::Weight,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(30), DOWNSCALE(248), DOWNSCALE(234), DOWNSCALE(16) },
					DataFieldCategory::PhysicalAppearance,
					DataFieldType::ImageField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(156), DOWNSCALE(328), DOWNSCALE(108), DOWNSCALE(16) },
					DataFieldCategory::ExpirationDate,
					DataFieldType::TextField
				},
			},
			9,
			type
		};
	case AppearanceType::CertificateOfVaccination:
		return DocLayout{
			{
				DataLayout{
					Rectangle{ DOWNSCALE(32),DOWNSCALE(102),DOWNSCALE(206),DOWNSCALE(12) },
					DataFieldCategory::Name,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(54),DOWNSCALE(128),DOWNSCALE(184),DOWNSCALE(12) },
					DataFieldCategory::PassportNumber,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(34),DOWNSCALE(190),DOWNSCALE(200),DOWNSCALE(12) },
					DataFieldCategory::Vaccination1,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(34),DOWNSCALE(214),DOWNSCALE(200),DOWNSCALE(12) },
					DataFieldCategory::Vaccination2,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(34),DOWNSCALE(238),DOWNSCALE(200),DOWNSCALE(12) },
					DataFieldCategory::Vaccination3,
					DataFieldType::TextField
				}
			},
			5,
			type
		};
	case AppearanceType::DiplomaticAuthorization:
		return DocLayout{
			{
				DataLayout{
					Rectangle{ DOWNSCALE(74),DOWNSCALE(182),DOWNSCALE(214),DOWNSCALE(12) },
					DataFieldCategory::Name,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(98), DOWNSCALE(206), DOWNSCALE(190), DOWNSCALE(12) },
					DataFieldCategory::PassportNumber,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(68), DOWNSCALE(8), DOWNSCALE(140),DOWNSCALE(12) },
					DataFieldCategory::IssuingCountry,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(42), DOWNSCALE(304),DOWNSCALE(240), DOWNSCALE(56) },
					DataFieldCategory::CountryList,
					DataFieldType::TextField
				}
			},
			4,
			type
		};
	case AppearanceType::EntryPermit:
		return DocLayout{
			{
				DataLayout{
					Rectangle{ DOWNSCALE(30), DOWNSCALE(182), DOWNSCALE(238), DOWNSCALE(12) },
					DataFieldCategory::Name,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(30), DOWNSCALE(248), DOWNSCALE(238), DOWNSCALE(12) },
					DataFieldCategory::PassportNumber,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(102), DOWNSCALE(280), DOWNSCALE(166), DOWNSCALE(12) },
					DataFieldCategory::Purpose,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(102), DOWNSCALE(310), DOWNSCALE(166), DOWNSCALE(12) },
					DataFieldCategory::DurationOfStay,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(102), DOWNSCALE(340), DOWNSCALE(166), DOWNSCALE(12) },
					DataFieldCategory::ExpirationDate,
					DataFieldType::TextField
				}
			},
			5,
			type
		};
	case AppearanceType::EntryTicket:
		return DocLayout{
			{
				DataLayout{
					Rectangle{ DOWNSCALE(176), DOWNSCALE(64), DOWNSCALE(90), DOWNSCALE(16) },
					DataFieldCategory::ValidDate,
					DataFieldType::TextField
				},
			},
			1,
			type
		};
	case AppearanceType::GrantOfAsylum:
		return DocLayout{
			{
				DataLayout{
					Rectangle{ DOWNSCALE(144),DOWNSCALE(108),DOWNSCALE(154),DOWNSCALE(34) },
					DataFieldCategory::Name,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(20),DOWNSCALE(100),DOWNSCALE(120),DOWNSCALE(144) },
					DataFieldCategory::Photo,
					DataFieldType::ImageField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(182),DOWNSCALE(156),DOWNSCALE(116),DOWNSCALE(16) },
					DataFieldCategory::Nationality,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(182),DOWNSCALE(174),DOWNSCALE(116),DOWNSCALE(16) },
					DataFieldCategory::PassportNumber,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(182),DOWNSCALE(192),DOWNSCALE(116),DOWNSCALE(16) },
					DataFieldCategory::DateOfBirth,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(182),DOWNSCALE(210),DOWNSCALE(116),DOWNSCALE(16) },
					DataFieldCategory::Height,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(182),DOWNSCALE(228),DOWNSCALE(116),DOWNSCALE(16) },
					DataFieldCategory::Weight,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(22),DOWNSCALE(250),DOWNSCALE(278),DOWNSCALE(66) },
					DataFieldCategory::FingerPrints,
					DataFieldType::ImageField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(156),DOWNSCALE(326),DOWNSCALE(100),DOWNSCALE(16) },
					DataFieldCategory::ExpirationDate,
					DataFieldType::TextField
				},
			},
			9,
			type
		};
	case AppearanceType::IdentityCard:
		return DocLayout{
			{
				DataLayout{
					Rectangle{ DOWNSCALE(12), DOWNSCALE(22), DOWNSCALE(232), DOWNSCALE(12) },
					DataFieldCategory::District,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(100), DOWNSCALE(40), DOWNSCALE(144), DOWNSCALE(30) },
					DataFieldCategory::Name,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(130), DOWNSCALE(80), DOWNSCALE(114), DOWNSCALE(12) },
					DataFieldCategory::DateOfBirth,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(130), DOWNSCALE(100), DOWNSCALE(114), DOWNSCALE(12) },
					DataFieldCategory::Height,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(130), DOWNSCALE(120), DOWNSCALE(114), DOWNSCALE(12) },
					DataFieldCategory::Weight,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(12), DOWNSCALE(34), DOWNSCALE(78), DOWNSCALE(96) },
					DataFieldCategory::Photo,
					DataFieldType::ImageField
				},
			},
			6,
			type
		};
	case AppearanceType::IdentitySupplement:
		return DocLayout{
			{
				DataLayout{
					Rectangle{ DOWNSCALE(46), DOWNSCALE(64), DOWNSCALE(114), DOWNSCALE(16) },
					DataFieldCategory::Height,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(46), DOWNSCALE(86), DOWNSCALE(114), DOWNSCALE(16) },
					DataFieldCategory::Weight,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(20), DOWNSCALE(132), DOWNSCALE(142), DOWNSCALE(48) },
					DataFieldCategory::Description,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(52), DOWNSCALE(278), DOWNSCALE(108), DOWNSCALE(16) },
					DataFieldCategory::ExpirationDate,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(74), DOWNSCALE(182), DOWNSCALE(88), DOWNSCALE(70) },
					DataFieldCategory::ThumbPrint,
					DataFieldType::ImageField
				},
			},
			5,
			type
		};
	case AppearanceType::WorkPass:
		return DocLayout{
			{
				DataLayout{
					Rectangle{ DOWNSCALE(74), DOWNSCALE(134), DOWNSCALE(204), DOWNSCALE(12) },
					DataFieldCategory::Name,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(74), DOWNSCALE(164), DOWNSCALE(204), DOWNSCALE(12) },
					DataFieldCategory::Field,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(74), DOWNSCALE(194), DOWNSCALE(204), DOWNSCALE(12) },
					DataFieldCategory::EndDate,
					DataFieldType::TextField
				},
			},
			3,
			type
		};
	case AppearanceType::Passport_Antegria:
		return DocLayout{
			{
				DataLayout{
					Rectangle{ DOWNSCALE(50), DOWNSCALE(220), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::Sex,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(50), DOWNSCALE(238), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::IssuingCity,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(16), DOWNSCALE(278), DOWNSCALE(230), DOWNSCALE(16) },
					DataFieldCategory::Name,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(50), DOWNSCALE(202), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::DateOfBirth,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(50), DOWNSCALE(256), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::ExpirationDate,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(16), DOWNSCALE(298), DOWNSCALE(230), DOWNSCALE(16) },
					DataFieldCategory::PassportNumber,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(166), DOWNSCALE(176) , DOWNSCALE(80), DOWNSCALE(96) },
					DataFieldCategory::Photo,
					DataFieldType::ImageField
				},

			},
			7,
			type
		};
	case AppearanceType::Passport_Arstotzka:
		return DocLayout{
			{
				DataLayout{
					Rectangle{ DOWNSCALE(136), DOWNSCALE(212), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::Sex,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(136), DOWNSCALE(228), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::IssuingCity,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(16), DOWNSCALE(176), DOWNSCALE(230), DOWNSCALE(16) },
					DataFieldCategory::Name,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(136), DOWNSCALE(196), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::DateOfBirth,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(136), DOWNSCALE(244), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::ExpirationDate,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(16), DOWNSCALE(296), DOWNSCALE(124), DOWNSCALE(16) },
					DataFieldCategory::PassportNumber,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(16), DOWNSCALE(196), DOWNSCALE(80), DOWNSCALE(96) },
					DataFieldCategory::Photo,
					DataFieldType::ImageField
				},

			},
			7,
			type
		};
	case AppearanceType::Passport_Impor:
		return DocLayout{
			{
				DataLayout{
					Rectangle{ DOWNSCALE(140), DOWNSCALE(210), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::Sex,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(140), DOWNSCALE(226), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::IssuingCity,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(16), DOWNSCALE(172), DOWNSCALE(230), DOWNSCALE(16) },
					DataFieldCategory::Name,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(140), DOWNSCALE(194), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::DateOfBirth,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(140), DOWNSCALE(242), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::ExpirationDate,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(128), DOWNSCALE(292), DOWNSCALE(118), DOWNSCALE(16) },
					DataFieldCategory::PassportNumber,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(18), DOWNSCALE(192), DOWNSCALE(80), DOWNSCALE(96) },
					DataFieldCategory::Photo,
					DataFieldType::ImageField
				},

			},
			7,
			type
		};
	case AppearanceType::Passport_Kolechia:
		return DocLayout{
			{
				DataLayout{
					Rectangle{ DOWNSCALE(138), DOWNSCALE(230), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::Sex,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(138), DOWNSCALE(246), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::IssuingCity,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(16), DOWNSCALE(196), DOWNSCALE(230), DOWNSCALE(16) },
					DataFieldCategory::Name,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(138), DOWNSCALE(214), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::DateOfBirth,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(136), DOWNSCALE(262), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::ExpirationDate,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(134), DOWNSCALE(296), DOWNSCALE(118), DOWNSCALE(16) },
					DataFieldCategory::PassportNumber,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(16), DOWNSCALE(214), DOWNSCALE(80), DOWNSCALE(96) },
					DataFieldCategory::Photo,
					DataFieldType::ImageField
				},

			},
			7,
			type
		};
	case AppearanceType::Passport_Obristan:
		return DocLayout{
			{
				DataLayout{
					Rectangle{ DOWNSCALE(54), DOWNSCALE(238), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::Sex,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(54), DOWNSCALE(254), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::IssuingCity,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(16), DOWNSCALE(196), DOWNSCALE(230), DOWNSCALE(16) },
					DataFieldCategory::Name,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(54), DOWNSCALE(222), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::DateOfBirth,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(54), DOWNSCALE(270), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::ExpirationDate,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(20), DOWNSCALE(296), DOWNSCALE(118), DOWNSCALE(16) },
					DataFieldCategory::PassportNumber,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(168), DOWNSCALE(214), DOWNSCALE(80), DOWNSCALE(96) },
					DataFieldCategory::Photo,
					DataFieldType::ImageField
				},

			},
			7,
			type
		};
	case AppearanceType::Passport_Republia:
		return DocLayout{
			{
				DataLayout{
					Rectangle{ DOWNSCALE(54), DOWNSCALE(212), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::Sex,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(54), DOWNSCALE(228), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::IssuingCity,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(16), DOWNSCALE(174), DOWNSCALE(230), DOWNSCALE(16) },
					DataFieldCategory::Name,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(54), DOWNSCALE(196), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::DateOfBirth,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(54), DOWNSCALE(244), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::ExpirationDate,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(136), DOWNSCALE(296), DOWNSCALE(118), DOWNSCALE(16) },
					DataFieldCategory::PassportNumber,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(170), DOWNSCALE(192), DOWNSCALE(80), DOWNSCALE(96) },
					DataFieldCategory::Photo,
					DataFieldType::ImageField
				},

			},
			7,
			type
		};
	case AppearanceType::Passport_UnitedFederation:
		return DocLayout{
			{
				DataLayout{
					Rectangle{ DOWNSCALE(138), DOWNSCALE(228), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::Sex,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(138), DOWNSCALE(244), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::IssuingCity,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(16), DOWNSCALE(196), DOWNSCALE(230), DOWNSCALE(16) },
					DataFieldCategory::Name,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(138), DOWNSCALE(212), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::DateOfBirth,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(138), DOWNSCALE(260), DOWNSCALE(106), DOWNSCALE(16) },
					DataFieldCategory::ExpirationDate,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(134), DOWNSCALE(296), DOWNSCALE(118), DOWNSCALE(16) },
					DataFieldCategory::PassportNumber,
					DataFieldType::TextField
				},
				DataLayout{
					Rectangle{ DOWNSCALE(16), DOWNSCALE(212), DOWNSCALE(80), DOWNSCALE(96) },
					DataFieldCategory::Photo,
					DataFieldType::ImageField
				},

			},
			7,
			type
		};
	case AppearanceType::Invalid:
	default:
		return {};
	}
}

///////////////////////////// public methods //////////

CONSTEXPR Rectangle DocLayout::GetFieldBox(DataFieldCategory type) {
	for (size_t i = 0; i < m_layoutCount; i++) {
		if (m_layouts[i].GetCategory() == type) {
			return m_layouts[i].GetBox();
		}
	}
	return Rectangle{};
}

CONSTEXPR const std::array<Rectangle, 9> DocLayout::GetAllFieldBoxes() {
	std::array<Rectangle, 9> boxes{};
	for (size_t i = 0; i < m_layoutCount; i++) {
		boxes[i] = m_layouts[i].GetBox();
	}
	return boxes;
}

CONSTEXPR const DocLayout::DataLayout DocLayout::GetLayout(DataFieldCategory type) const noexcept {
	for (size_t i = 0; i < m_layoutCount; i++) {
		if (m_layouts[i].GetCategory() == type) {
			return m_layouts[i];
		}
	}
	return {};
}

CONSTEXPR const DocLayout::DataLayout* DocLayout::GetAllLayouts() const noexcept {
	return m_layouts.data();
}

CONSTEXPR const size_t DocLayout::GetLayoutCount() const noexcept {
	return m_layoutCount;
}

#undef CONSTEXPR

} // namespace Documents::V2
