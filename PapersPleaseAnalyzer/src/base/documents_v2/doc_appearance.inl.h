#pragma once

namespace Documents::V2 {

	constexpr DocAppearance::DocAppearance() noexcept
		: m_borderColors{}, m_colorCount{}, m_shape{}, m_appearanceType{}
	{ }

	constexpr DocAppearance::DocAppearance(std::array<RgbColor, 4> borderColors, size_t colorCount, Shape shape, AppearanceType appearanceType) noexcept
		: m_borderColors{ borderColors }, m_colorCount{ colorCount }, m_shape{ shape }, m_appearanceType{ appearanceType }
	{ }

	constexpr size_t DocAppearance::CountValidColors(std::array<RgbColor, 4> borderColors) noexcept
	{
		size_t count = 0;
		for (size_t i = 0; i < borderColors.size(); i++)
		{
			count += (borderColors[i].blue + borderColors[i].green + borderColors[i].red) != 0;
		}
		return count;
	}

	constexpr const RgbColor* DocAppearance::GetColors() const noexcept
	{
		return m_borderColors.data();
	}

	constexpr size_t DocAppearance::GetColorCount() const noexcept
	{
		return m_colorCount;
	}

	constexpr const Shape DocAppearance::GetShape() const noexcept
	{
		return m_shape;
	}

	constexpr const int DocAppearance::GetWidth() const noexcept
	{
		return m_shape.width;
	}

	constexpr const int DocAppearance::GetHeight() const noexcept
	{
		return m_shape.height;
	}

	constexpr const AppearanceType DocAppearance::GetType() const noexcept
	{
		return m_appearanceType;
	}

	constexpr DocAppearance DocAppearance::Get(AppearanceType type) noexcept
	{
		switch (type)
		{
			case AppearanceType::AccessPermit:
				return DocAppearance{
					{
						RgbColor{ 215, 233, 210 },
						RgbColor{ 176, 168, 173 },
					},
					2,
					Shape{ DOWNSCALE(294), DOWNSCALE(392) },
					type
				};
			case AppearanceType::CertificateOfVaccination:
				return DocAppearance{
					{
						RgbColor{ 234, 223, 128 },
						RgbColor{ 177, 151, 97 },
					},
					2,
					Shape{ DOWNSCALE(270), DOWNSCALE(312) },
					type
				};
			case AppearanceType::DiplomaticAuthorization:
				return DocAppearance{
					{
						RgbColor{ 238, 238, 251 },
						RgbColor{ 190, 202, 209 },
						RgbColor{ 154, 179, 168 },
					},
					3,
					Shape{ DOWNSCALE(300), DOWNSCALE(400) },
					type
				};
			case AppearanceType::EntryPermit:
				return DocAppearance{
					{
						RgbColor{ 224, 233, 199 },
						RgbColor{ 202, 201, 175 },
					},
					2,
					Shape{ DOWNSCALE(300), DOWNSCALE(402) },
					type
				};
			case AppearanceType::EntryTicket:
				return DocAppearance{
					{
						RgbColor{ 224, 233, 199 },
						RgbColor{ 180, 169, 151 },
					},
					2,
					Shape{ DOWNSCALE(280), DOWNSCALE(102) },
					type
				};
			case AppearanceType::GrantOfAsylum:
				return DocAppearance{
					{
						RgbColor{ 253, 222, 223 },
						RgbColor{ 180, 157, 175 },
					},
					2,
					Shape{ DOWNSCALE(320), DOWNSCALE(374) },
					type
				};
			case AppearanceType::IdentityCard:
				return DocAppearance{
					{
						RgbColor{ 217, 189, 247 },
						RgbColor{ 178, 156, 204 },
					},
					2,
					Shape{ DOWNSCALE(252), DOWNSCALE(142) },
					type
				};
			case AppearanceType::IdentitySupplement:
				return DocAppearance{
					{
						RgbColor{ 232, 218, 199 },
						RgbColor{ 181, 167, 166 },
					},
					2,
					Shape{ DOWNSCALE(180), DOWNSCALE(300) },
					type
				};
			case AppearanceType::Passport_Antegria:
				return DocAppearance{
					{
						RgbColor{ 49, 77, 33 }
					},
					1,
					Shape{ DOWNSCALE(260), DOWNSCALE(324) },
					type
				};
			case AppearanceType::Passport_Arstotzka:
				return DocAppearance{
					{
						RgbColor{ 59, 72, 59 }
					},
					1,
					Shape{ DOWNSCALE(260), DOWNSCALE(324) },
					type
				};
			case AppearanceType::Passport_Impor:
				return DocAppearance{
					{
						RgbColor{ 102, 31, 9 }
					},
					1,
					Shape{ DOWNSCALE(260), DOWNSCALE(324) },
					type
				};
			case AppearanceType::Passport_Kolechia:
				return DocAppearance{
					{
						RgbColor{ 85, 37, 63 }
					},
					1,
					Shape{ DOWNSCALE(260), DOWNSCALE(324) },
					type
				};
			case AppearanceType::Passport_Obristan:
				return DocAppearance{
					{
						RgbColor{ 138, 12, 12 }
					},
					1,
					Shape{ DOWNSCALE(260), DOWNSCALE(324) },
					type
				};
			case AppearanceType::Passport_Republia:
				return DocAppearance{
					{
						RgbColor{ 76, 42, 27 }
					},
					1,
					Shape{ DOWNSCALE(260), DOWNSCALE(324) },
					type
				};
			case AppearanceType::Passport_UnitedFederation:
				return DocAppearance{
					{
						RgbColor{ 35, 30, 85 }
					},
					1,
					Shape{ DOWNSCALE(260), DOWNSCALE(324) },
					type
				};
			case AppearanceType::WorkPass:
				return DocAppearance{
					{
						RgbColor{ 233, 199, 211 },
						RgbColor{ 204, 175, 192 },
					},
					1,
					Shape{ DOWNSCALE(294), DOWNSCALE(270) },
					type
				};
			case AppearanceType::Invalid:
			default:
				return {};
		}
	}

}