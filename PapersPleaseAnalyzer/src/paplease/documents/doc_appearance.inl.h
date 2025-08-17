#pragma once

namespace paplease {
	namespace documents {

#pragma region Constructors

		constexpr DocAppearance::DocAppearance() noexcept
			: m_borderColors{}, m_colorCount{}, m_shape{}, m_appearanceType{}
		{}

		constexpr DocAppearance::DocAppearance(
			const std::array<RgbColor, DocAppearance::MaxBorderColors>& borderColors,
			const size_t colorCount,
			const ScaledShape shape,
			const AppearanceType appearanceType) noexcept
			: m_borderColors{ borderColors },
			m_colorCount{ colorCount },
			m_shape{ static_cast<Shape>(shape) },
			m_appearanceType{ appearanceType }
		{}

#pragma endregion

#pragma region Getters

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

#pragma endregion

#pragma region Object Acquisition

		constexpr const DocAppearance DocAppearance::Get(AppearanceType type) noexcept
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
						ScaledShape{ 294, 392 },
						type
					};
				case AppearanceType::CertificateOfVaccination:
					return DocAppearance{
						{
							RgbColor{ 234, 223, 128 },
							RgbColor{ 177, 151, 97 },
						},
						2,
						ScaledShape{ 270, 312 },
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
						ScaledShape{ 300, 400 },
						type
					};
				case AppearanceType::EntryPermit:
					return DocAppearance{
						{
							RgbColor{ 224, 233, 199 },
							RgbColor{ 202, 201, 175 },
						},
						2,
						ScaledShape{ 300, 402 },
						type
					};
				case AppearanceType::EntryTicket:
#if EXPERIMENTAL_MATCH_ONE_COLOR
					return DocAppearance{
						{
							RgbColor{ 137, 106, 103 },
						},
						1,
						ScaledShape{ 256, 88 },
						type
					};
#else
					return DocAppearance{
						{
							RgbColor{ 224, 233, 199 },
							RgbColor{ 180, 169, 151 },
						},
						2,
						ScaledShape{ 280, 102 },
						type
					};
#endif
				case AppearanceType::GrantOfAsylum:
					return DocAppearance{
						{
							RgbColor{ 253, 222, 223 },
							RgbColor{ 180, 157, 175 },
						},
						2,
						ScaledShape{ 320, 374 },
						type
					};
				case AppearanceType::IdentityCard:
					return DocAppearance{
						{
							RgbColor{ 217, 189, 247 },
							RgbColor{ 178, 156, 204 },
						},
						2,
						ScaledShape{ 252, 142 },
						type
					};
				case AppearanceType::IdentitySupplement:
					return DocAppearance{
						{
							RgbColor{ 232, 218, 199 },
							RgbColor{ 181, 167, 166 },
						},
						2,
						ScaledShape{ 180, 300 },
						type
					};
				case AppearanceType::Passport_Antegria:
					return DocAppearance{
						{
							RgbColor{ 49, 77, 33 }
						},
						1,
						ScaledShape{ 260, 324 },
						type
					};
				case AppearanceType::Passport_Arstotzka:
					return DocAppearance{
						{
							RgbColor{ 59, 72, 59 }
						},
						1,
						ScaledShape{ 260, 324 },
						type
					};
				case AppearanceType::Passport_Impor:
					return DocAppearance{
						{
							RgbColor{ 102, 31, 9 }
						},
						1,
						ScaledShape{ 260, 324 },
						type
					};
				case AppearanceType::Passport_Kolechia:
					return DocAppearance{
						{
							RgbColor{ 85, 37, 63 }
						},
						1,
						ScaledShape{ 260, 324 },
						type
					};
				case AppearanceType::Passport_Obristan:
					return DocAppearance{
						{
							RgbColor{ 138, 12, 12 }
						},
						1,
						ScaledShape{ 260, 324 },
						type
					};
				case AppearanceType::Passport_Republia:
					return DocAppearance{
						{
							RgbColor{ 76, 42, 27 }
						},
						1,
						ScaledShape{ 260, 324 },
						type
					};
				case AppearanceType::Passport_UnitedFederation:
					return DocAppearance{
						{
							RgbColor{ 35, 30, 85 }
						},
						1,
						ScaledShape{ 260, 324 },
						type
					};
				case AppearanceType::WorkPass:
					return DocAppearance{
						{
							RgbColor{ 233, 199, 211 },
							RgbColor{ 204, 175, 192 },
						},
						2,
						ScaledShape{ 294, 270 },
						type
					};
				case AppearanceType::RuleBook:
					return DocAppearance{
						{
							RgbColor{87, 72, 72}
						},
						1,
						ScaledShape{ 480, 320 },
						AppearanceType::RuleBook
					};
				case AppearanceType::Bulletin:
					return DocAppearance{
						{
							RgbColor{240, 240, 240}
						},
						1,
						ScaledShape{ 298, 398 },
						AppearanceType::Bulletin
					};
				case AppearanceType::Transcript:
					return DocAppearance{
						{
							RgbColor{210, 237, 236}
						},
						1,
						ScaledShape{ 300, 400 },
						AppearanceType::Transcript
					};
				case AppearanceType::Invalid:
					return {};
				default:
					std::cerr << "AppearanceType not implemented in DocAppearance::Get()!\n";
					return {};
			}
		}

		consteval const DocAppearance DocAppearance::GetInstant(AppearanceType type) noexcept
		{
			return DocAppearance::Get(type);
		}

#pragma endregion

	}  // namespace documents
}  // namespace paplease
