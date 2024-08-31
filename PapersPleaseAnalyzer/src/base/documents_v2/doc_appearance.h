#pragma once
#include <array>

#include "base/color.h"
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
	WorkPass,
	RuleBook,
	Bulletin,
	Transcript = 12,
	Passport_Antegria = 13,
	Passport_Arstotzka,
	Passport_Impor,
	Passport_Kolechia,
	Passport_Obristan,
	Passport_Republia,
	Passport_UnitedFederation,
};

class DocAppearance {
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
	constexpr DocAppearance(const std::array<RgbColor, 4>& borderColors, const size_t colorCount, const Shape shape, const AppearanceType appearanceType) noexcept;
private:
	const std::array<RgbColor, 4> m_borderColors;
	const size_t m_colorCount;
	const Shape m_shape;
	const AppearanceType m_appearanceType;
};

}

#include "base/documents_v2/doc_appearance.inl.h"