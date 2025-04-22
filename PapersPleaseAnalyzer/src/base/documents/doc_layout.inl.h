#pragma once

namespace paplease {
	namespace documents {
#pragma region DataLayout

#pragma region Constructors

			constexpr DocLayout::DataLayout::DataLayout() noexcept
				: m_dataBox{}, m_dataFieldCategory{}, m_dataFieldType{}
			{}

			constexpr DocLayout::DataLayout::DataLayout(ScaledRectangle dataBox, FieldCategory dataFieldCategory, FieldType dataFieldType) noexcept
				: m_dataBox{ static_cast<Rectangle>(dataBox) }, m_dataFieldCategory{ dataFieldCategory }, m_dataFieldType{ dataFieldType }
			{}

#pragma endregion

#pragma region Getters

			constexpr const Rectangle DocLayout::DataLayout::GetBox() const noexcept
			{
				return m_dataBox;
			}
			constexpr const FieldCategory DocLayout::DataLayout::GetCategory() const noexcept
			{
				return m_dataFieldCategory;
			}
			constexpr const FieldType DocLayout::DataLayout::GetType() const noexcept
			{
				return m_dataFieldType;
			}

#pragma endregion

#pragma endregion

#pragma region Constructors

			constexpr DocLayout::DocLayout() noexcept
				: m_layouts{}, m_layoutCount{}
			{}

			constexpr DocLayout::DocLayout(std::array<DataLayout, DocLayout::MaxLayouts> layouts, size_t layoutCount) noexcept
				: m_layouts{ layouts }, m_layoutCount{ layoutCount }
			{}

#pragma endregion

#pragma region Getters

			constexpr Rectangle DocLayout::GetFieldBox(FieldCategory type)
			{
				for (size_t i = 0; i < m_layoutCount; i++)
				{
					if (m_layouts[i].GetCategory() == type)
					{
						return m_layouts[i].GetBox();
					}
				}
				return Rectangle{};
			}

			constexpr const std::array<Rectangle, DocLayout::MaxLayouts> DocLayout::GetAllFieldBoxes()
			{
				std::array<Rectangle, DocLayout::MaxLayouts> boxes{};
				for (size_t i = 0; i < m_layoutCount; i++)
				{
					boxes[i] = m_layouts[i].GetBox();
				}
				return boxes;
			}

			constexpr const DocLayout::DataLayout& DocLayout::GetLayout(FieldCategory type) const noexcept
			{
				for (size_t i = 0; i < m_layoutCount; i++)
				{
					if (m_layouts[i].GetCategory() == type)
					{
						return m_layouts[i];
					}
				}
				assert(false, "I don't know what the line under does?");
				//return m_layouts[(size_t)FieldCategory::Invalid];
			}

			constexpr const DocLayout::DataLayout* DocLayout::GetAllLayouts() const noexcept
			{
				return m_layouts.data();
			}

			constexpr const size_t DocLayout::GetLayoutCount() const noexcept
			{
				return m_layoutCount;
			}

#pragma endregion

#pragma region Object Acquisition

			constexpr const DocLayout DocLayout::Get(AppearanceType type) noexcept
			{
				switch (type)
				{
					case AppearanceType::AccessPermit:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 30, 108, 234, 16 },
									FieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 30, 152, 112, 16 },
									FieldCategory::IssuingCountry,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 156, 152, 116, 16 },
									FieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 30, 196, 112, 16 },
									FieldCategory::Purpose,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 156, 196, 112, 16 },
									FieldCategory::DurationOfStay,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 30, 240, 112, 16 },
									FieldCategory::Height,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 156, 240, 112, 16 },
									FieldCategory::Weight,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 30, 284, 234, 16 },
									FieldCategory::PhysicalAppearance,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 156, 328, 108, 16 },
									FieldCategory::ExpirationDate,
									FieldType::Text
								},
							},
							9,
						};
					case AppearanceType::CertificateOfVaccination:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 32, 102, 206, 12 },
									FieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 54, 128, 184, 12 },
									FieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 34, 190, 200, 12 },
									FieldCategory::Vaccination1,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 34, 214, 200, 12 },
									FieldCategory::Vaccination2,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 34, 238, 200, 12 },
									FieldCategory::Vaccination3,
									FieldType::Text
								}
							},
							5,
						};
					case AppearanceType::DiplomaticAuthorization:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 74, 182, 214, 12 },
									FieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 98, 206, 190, 12 },
									FieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 68, 8, 140, 12 },
									FieldCategory::IssuingCountry,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 42, 304, 240, 56 },
									FieldCategory::CountryList,
									FieldType::Text
								}
							},
							4,
						};
					case AppearanceType::EntryPermit:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 30, 182, 238, 12 },
									FieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 30, 248, 238, 12 },
									FieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 102, 280, 166, 12 },
									FieldCategory::Purpose,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 102, 310, 166, 12 },
									FieldCategory::DurationOfStay,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 102, 340, 166, 12 },
									FieldCategory::ExpirationDate,
									FieldType::Text
								}
							},
							5,
						};
					case AppearanceType::EntryTicket:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 176, 64, 90, 16 },
									FieldCategory::ValidDate,
									FieldType::Text
								},
							},
							1,
						};
					case AppearanceType::GrantOfAsylum:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 144, 108, 154, 34 },
									FieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 20, 100, 120, 144 },
									FieldCategory::Photo,
									FieldType::Image
								},
								DataLayout{
									ScaledRectangle{ 182, 156, 116, 16 },
									FieldCategory::IssuingCountry,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 182, 174, 116, 16 },
									FieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 182, 192, 116, 16 },
									FieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 182, 210, 116, 16 },
									FieldCategory::Height,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 182, 228, 116, 16 },
									FieldCategory::Weight,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 22, 250, 278, 66 },
									FieldCategory::FingerPrints,
									FieldType::Image
								},
								DataLayout{
									ScaledRectangle{ 156, 326, 100, 16 },
									FieldCategory::ExpirationDate,
									FieldType::Text
								},
							},
							9,
						};
					case AppearanceType::IdentityCard:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 12, 22, 232, 12 },
									FieldCategory::District,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 100, 40, 144, 30 },
									FieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 130, 80, 114, 12 },
									FieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 130, 100, 114, 12 },
									FieldCategory::Height,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 130, 120, 114, 12 },
									FieldCategory::Weight,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 12, 34, 78, 96 },
									FieldCategory::Photo,
									FieldType::Image
								},
							},
							6,
						};
					case AppearanceType::IdentitySupplement:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 46, 64, 114, 16 },
									FieldCategory::Height,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 46, 86, 114, 16 },
									FieldCategory::Weight,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 20, 132, 142, 48 },
									FieldCategory::PhysicalAppearance,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 52, 278, 108, 16 },
									FieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 74, 182, 88, 70 },
									FieldCategory::ThumbPrint,
									FieldType::Image
								},
							},
							5,
						};
					case AppearanceType::WorkPass:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 74, 134, 204, 12 },
									FieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 74, 164, 204, 12 },
									FieldCategory::Field,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 74, 194, 204, 12 },
									FieldCategory::EndDate,
									FieldType::Text
								},
							},
							3,
						};
					case AppearanceType::Passport_Antegria:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 50, 220, 116, 16 },
									FieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 50, 238, 116, 16 },
									FieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 16, 278, 230, 16 },
									FieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 50, 202, 116, 16 },
									FieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 50, 256, 116, 16 },
									FieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 16, 298, 230, 16 },
									FieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 166, 176, 80, 96 },
									FieldCategory::Photo,
									FieldType::Image
								},

							},
							7,
						};
					case AppearanceType::Passport_Arstotzka:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 136, 212, 116, 16 },
									FieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 136, 228, 116, 16 },
									FieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 16, 176, 230, 16 },
									FieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 136, 196, 116, 16 },
									FieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 136, 244, 116, 16 },
									FieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 16, 296, 124, 16 },
									FieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 16, 196, 80, 96 },
									FieldCategory::Photo,
									FieldType::Image
								},

							},
							7,
						};
					case AppearanceType::Passport_Impor:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 140, 210, 116, 16 },
									FieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 140, 226, 116, 16 },
									FieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 16, 172, 230, 16 },
									FieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 140, 194, 116, 16 },
									FieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 140, 242, 116, 16 },
									FieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 128, 292, 118, 16 },
									FieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 18, 192, 80, 96 },
									FieldCategory::Photo,
									FieldType::Image
								},

							},
							7,
						};
					case AppearanceType::Passport_Kolechia:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 138, 230, 116, 16 },
									FieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 138, 246, 116, 16 },
									FieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 16, 196, 230, 16 },
									FieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 138, 214, 116, 16 },
									FieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 136, 262, 116, 16 },
									FieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 130, 296, 118, 16 },
									FieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 16, 214, 80, 96 },
									FieldCategory::Photo,
									FieldType::Image
								},

							},
							7,
						};
					case AppearanceType::Passport_Obristan:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 54, 238, 116, 16 },
									FieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 54, 254, 116, 16 },
									FieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 16, 196, 230, 16 },
									FieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 54, 222, 116, 16 },
									FieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 54, 270, 116, 16 },
									FieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 20, 296, 118, 16 },
									FieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 168, 214, 80, 96 },
									FieldCategory::Photo,
									FieldType::Image
								},
							},
							7,
						};
					case AppearanceType::Passport_Republia:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 54, 212, 116, 16 },
									FieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 54, 228, 116, 16 },
									FieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 16, 174, 230, 16 },
									FieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 54, 196, 116, 16 },
									FieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 54, 244, 116, 16 },
									FieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 130, 296, 118, 16 },
									FieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 170, 192, 80, 96 },
									FieldCategory::Photo,
									FieldType::Image
								},

							},
							7,
						};
					case AppearanceType::Passport_UnitedFederation:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 138, 228, 116, 16 },
									FieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 138, 244, 116, 16 },
									FieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 16, 196, 230, 16 },
									FieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 138, 212, 116, 16 },
									FieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 138, 260, 116, 16 },
									FieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 130, 296, 118, 16 },
									FieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 16, 212, 80, 96 },
									FieldCategory::Photo,
									FieldType::Image
								},

							},
							7,
						};
					case AppearanceType::RuleBook:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 32, 50, 188, 38 },
									FieldCategory::Rule1,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 32, 98, 188, 38 },
									FieldCategory::Rule2,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 32, 146, 188, 38 },
									FieldCategory::Rule3,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 32, 194, 188, 38 },
									FieldCategory::Rule4,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 32, 242, 188, 38 },
									FieldCategory::Rule5,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 274, 50, 188, 38 },
									FieldCategory::Rule6,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 274, 98, 188, 38 },
									FieldCategory::Rule7,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 274, 146, 188, 38 },
									FieldCategory::Rule8,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 274, 194, 188, 38 },
									FieldCategory::Rule9,
									FieldType::Text
								},
								DataLayout{
									ScaledRectangle{ 274, 242, 188, 38 },
									FieldCategory::Rule10,
									FieldType::Text
								}
							},
							10
						};
					case AppearanceType::Bulletin:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 38, 92, 64, 78 },
									FieldCategory::CriminalPhoto1,
									FieldType::Image
								},
								DataLayout{
									ScaledRectangle{ 38, 190, 64, 78 },
									FieldCategory::CriminalPhoto2,
									FieldType::Image
								},
								DataLayout{
									ScaledRectangle{ 38, 288, 64, 78 },
									FieldCategory::CriminalPhoto3,
									FieldType::Image
								}
							},
							3
						};
					case AppearanceType::Transcript:
						return DocLayout{
							{
								DataLayout{
									ScaledRectangle{ 14, 32, 272, 362 },
									FieldCategory::TranscriptPage,
									FieldType::Text
								}
							},
							1
						};
					case AppearanceType::Invalid:
						return {};
					default:
					{
						assert(false);
					}
				}
			}

			consteval const DocLayout DocLayout::GetInstant(AppearanceType type) noexcept
			{
				return DocLayout::Get(type);
			}

			consteval const DocLayout DocLayout::GetBooth() noexcept
			{

				return DocLayout{
					{
						DataLayout{
							ScaledRectangle{ 8, 418, 56, 12 },
							FieldCategory::BoothDate,
							FieldType::Text
						},
						DataLayout{
							ScaledRectangle{ 168, 416, 22, 12 },
							FieldCategory::BoothCounter,
							FieldType::Text
						},
						DataLayout{
							ScaledRectangle{ 0, 0, 356, 214 },
							FieldCategory::Photo,
							FieldType::Image
						},
						DataLayout{
							ScaledRectangle{ 302, 390, 44, 12 },
							FieldCategory::Weight,
							FieldType::Text
						},
					},
					4
				};
			}

#pragma endregion

	}  // namespace documents
}  // namespace paplease
