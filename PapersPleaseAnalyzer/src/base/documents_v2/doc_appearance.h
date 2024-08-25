#pragma once
#include <array>

#include "base/color.h"
#include "base/common.h"
#include "base/shape.h"

namespace Documents::V2 {

enum class AppearanceType {
	Invalid = 0,
	AccessPermit,
	CertificateOfVaccination,
	DiplomaticAuthorization,
	EntryPermit,
	EntryTicket,
	GrantOfAsylum,
	IdentityCard,
	IdentitySupplement,
	WorkPass = 9,
	Passport_Antegria = 10,
	Passport_Arstotzka,
	Passport_Impor,
	Passport_Kolechia,
	Passport_Obristan,
	Passport_Republia,
	Passport_UnitedFederation,
};

class DocAppearance {
public:
	[[nodiscard]] static constexpr DocAppearance Get(AppearanceType type) noexcept;
public:
	constexpr DocAppearance() noexcept;

	[[nodiscard]] constexpr const RgbColor* GetColors() const noexcept;
	[[nodiscard]] constexpr size_t GetColorCount() const noexcept;
	[[nodiscard]] constexpr const Shape GetShape() const noexcept;
	[[nodiscard]] constexpr const int GetWidth() const noexcept;
	[[nodiscard]] constexpr const int GetHeight() const noexcept;
	[[nodiscard]] constexpr const AppearanceType GetType() const noexcept;
private:
	constexpr DocAppearance(std::array<RgbColor, 4> borderColors, size_t colorCount, Shape shape, AppearanceType appearanceType) noexcept;

	constexpr size_t CountValidColors(std::array<RgbColor, 4> borderColors) noexcept;
private:
	std::array<RgbColor, 4> m_borderColors;
	size_t m_colorCount;
	Shape m_shape;
	AppearanceType m_appearanceType;
};

}

#include "base/documents_v2/doc_appearance.inl.h"