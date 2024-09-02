#pragma once

namespace paplease {
	namespace documents {
		namespace v2 {

#pragma region DataLayout

#pragma region Constructors

			constexpr DocLayout::DataLayout::DataLayout() noexcept
				: m_dataBox{}, m_dataFieldCategory{}, m_dataFieldType{}
			{}

			constexpr DocLayout::DataLayout::DataLayout(Rectangle dataBox, DataFieldCategory dataFieldCategory, FieldType dataFieldType) noexcept
				: m_dataBox{ dataBox }, m_dataFieldCategory{ dataFieldCategory }, m_dataFieldType{ dataFieldType }
			{}

#pragma endregion

#pragma region Getters

			constexpr const Rectangle DocLayout::DataLayout::GetBox() const noexcept
			{
				return m_dataBox;
			}
			constexpr const DataFieldCategory DocLayout::DataLayout::GetCategory() const noexcept
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

			constexpr Rectangle DocLayout::GetFieldBox(DataFieldCategory type)
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

			constexpr const DocLayout::DataLayout DocLayout::GetLayout(DataFieldCategory type) const noexcept
			{
				for (size_t i = 0; i < m_layoutCount; i++)
				{
					if (m_layouts[i].GetCategory() == type)
					{
						return m_layouts[i];
					}
				}
				return m_layouts[(size_t)DataFieldCategory::Invalid];
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
									Rectangle{ 30, 108, 234, 16 },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 30, 152, 112, 16 },
									DataFieldCategory::Nationality,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 156, 152, 112, 16 },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 30, 196, 112, 16 },
									DataFieldCategory::Purpose,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 156, 196, 112, 16 },
									DataFieldCategory::DurationOfStay,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 30, 240, 112, 16 },
									DataFieldCategory::Height,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 156, 240, 112, 16 },
									DataFieldCategory::Weight,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 30, 284, 234, 16 },
									DataFieldCategory::PhysicalAppearance,
									FieldType::Image
								},
								DataLayout{
									Rectangle{ 156, 328, 108, 16 },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
							},
							9,
							//type
						};
					case AppearanceType::CertificateOfVaccination:
						return DocLayout{
							{
								DataLayout{
									Rectangle{ 32,102,206,12 },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 54,128,184,12 },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 34,190,200,12 },
									DataFieldCategory::Vaccination1,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 34,214,200,12 },
									DataFieldCategory::Vaccination2,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 34,238,200,12 },
									DataFieldCategory::Vaccination3,
									FieldType::Text
								}
							},
							5,
							// type
						};
					case AppearanceType::DiplomaticAuthorization:
						return DocLayout{
							{
								DataLayout{
									Rectangle{ 74,182,214,12 },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 98, 206, 190, 12 },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 68, 8, 140,12 },
									DataFieldCategory::IssuingCountry,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 42, 304,240, 56 },
									DataFieldCategory::CountryList,
									FieldType::Text
								}
							},
							4,
							// type
						};
					case AppearanceType::EntryPermit:
						return DocLayout{
							{
								DataLayout{
									Rectangle{ 30, 182, 238, 12 },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 30, 248, 238, 12 },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 102, 280, 166, 12 },
									DataFieldCategory::Purpose,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 102, 310, 166, 12 },
									DataFieldCategory::DurationOfStay,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 102, 340, 166, 12 },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								}
							},
							5,
							// type
						};
					case AppearanceType::EntryTicket:
						return DocLayout{
							{
								DataLayout{
									Rectangle{ 176, 64, 90, 16 },
									DataFieldCategory::ValidDate,
									FieldType::Text
								},
							},
							1,
							// type
						};
					case AppearanceType::GrantOfAsylum:
						return DocLayout{
							{
								DataLayout{
									Rectangle{ 144,108,154,34 },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 20,100,120,144 },
									DataFieldCategory::Photo,
									FieldType::Image
								},
								DataLayout{
									Rectangle{ 182,156,116,16 },
									DataFieldCategory::Nationality,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 182,174,116,16 },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 182,192,116,16 },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 182,210,116,16 },
									DataFieldCategory::Height,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 182,228,116,16 },
									DataFieldCategory::Weight,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 22,250,278,66 },
									DataFieldCategory::FingerPrints,
									FieldType::Image
								},
								DataLayout{
									Rectangle{ 156,326,100,16 },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
							},
							9,
							// type
						};
					case AppearanceType::IdentityCard:
						return DocLayout{
							{
								DataLayout{
									Rectangle{ 12, 22, 232, 12 },
									DataFieldCategory::District,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 100, 40, 144, 30 },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 130, 80, 114, 12 },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 130, 100, 114, 12 },
									DataFieldCategory::Height,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 130, 120, 114, 12 },
									DataFieldCategory::Weight,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 12, 34, 78, 96 },
									DataFieldCategory::Photo,
									FieldType::Image
								},
							},
							6,
							// type
						};
					case AppearanceType::IdentitySupplement:
						return DocLayout{
							{
								DataLayout{
									Rectangle{ 46, 64, 114, 16 },
									DataFieldCategory::Height,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 46, 86, 114, 16 },
									DataFieldCategory::Weight,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 20, 132, 142, 48 },
									DataFieldCategory::Description,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 52, 278, 108, 16 },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 74, 182, 88, 70 },
									DataFieldCategory::ThumbPrint,
									FieldType::Image
								},
							},
							5,
							// type
						};
					case AppearanceType::WorkPass:
						return DocLayout{
							{
								DataLayout{
									Rectangle{ 74, 134, 204, 12 },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 74, 164, 204, 12 },
									DataFieldCategory::Field,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 74, 194, 204, 12 },
									DataFieldCategory::EndDate,
									FieldType::Text
								},
							},
							3,
							// type
						};
					case AppearanceType::Passport_Antegria:
						return DocLayout{
							{
								DataLayout{
									Rectangle{ 50, 220, 106, 16 },
									DataFieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 50, 238, 106, 16 },
									DataFieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 16, 278, 230, 16 },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 50, 202, 106, 16 },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 50, 256, 106, 16 },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 16, 298, 230, 16 },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 166, 176 , 80, 96 },
									DataFieldCategory::Photo,
									FieldType::Image
								},

							},
							7,
							// type
						};
					case AppearanceType::Passport_Arstotzka:
						return DocLayout{
							{
								DataLayout{
									Rectangle{ 136, 212, 114, 16 },
									DataFieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 136, 228, 114, 16 },
									DataFieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 16, 176, 230, 16 },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 136, 196, 114, 16 },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 136, 244, 114, 16 },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 16, 296, 124, 16 },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 16, 196, 80, 96 },
									DataFieldCategory::Photo,
									FieldType::Image
								},

							},
							7,
							// type
						};
					case AppearanceType::Passport_Impor:
						return DocLayout{
							{
								DataLayout{
									Rectangle{ 140, 210, 106, 16 },
									DataFieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 140, 226, 106, 16 },
									DataFieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 16, 172, 230, 16 },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 140, 194, 106, 16 },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 140, 242, 106, 16 },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 128, 292, 118, 16 },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 18, 192, 80, 96 },
									DataFieldCategory::Photo,
									FieldType::Image
								},

							},
							7,
							// type
						};
					case AppearanceType::Passport_Kolechia:
						return DocLayout{
							{
								DataLayout{
									Rectangle{ 138, 230, 106, 16 },
									DataFieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 138, 246, 106, 16 },
									DataFieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 16, 196, 230, 16 },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 138, 214, 106, 16 },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 136, 262, 106, 16 },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 134, 296, 118, 16 },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 16, 214, 80, 96 },
									DataFieldCategory::Photo,
									FieldType::Image
								},

							},
							7,
							// type
						};
					case AppearanceType::Passport_Obristan:
						return DocLayout{
							{
								DataLayout{
									Rectangle{ 54, 238, 106, 16 },
									DataFieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 54, 254, 106, 16 },
									DataFieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 16, 196, 230, 16 },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 54, 222, 106, 16 },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 54, 270, 106, 16 },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 20, 296, 118, 16 },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 168, 214, 80, 96 },
									DataFieldCategory::Photo,
									FieldType::Image
								},

							},
							7,
							// type
						};
					case AppearanceType::Passport_Republia:
						return DocLayout{
							{
								DataLayout{
									Rectangle{ 54, 212, 106, 16 },
									DataFieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 54, 228, 106, 16 },
									DataFieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 16, 174, 230, 16 },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 54, 196, 106, 16 },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 54, 244, 106, 16 },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 136, 296, 118, 16 },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 170, 192, 80, 96 },
									DataFieldCategory::Photo,
									FieldType::Image
								},

							},
							7,
							// type
						};
					case AppearanceType::Passport_UnitedFederation:
						return DocLayout{
							{
								DataLayout{
									Rectangle{ 138, 228, 106, 16 },
									DataFieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 138, 244, 106, 16 },
									DataFieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 16, 196, 230, 16 },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 138, 212, 106, 16 },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 138, 260, 106, 16 },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 134, 296, 118, 16 },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 16, 212, 80, 96 },
									DataFieldCategory::Photo,
									FieldType::Image
								},

							},
							7,
							// type
						};
					case AppearanceType::RuleBook:
						return DocLayout{
							{
								DataLayout{
									Rectangle{ 32, 50, 188, 38},
									DataFieldCategory::Rule1,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 32, 98, 188, 38},
									DataFieldCategory::Rule2,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 32, 146, 188, 38},
									DataFieldCategory::Rule3,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 32, 194, 188, 38},
									DataFieldCategory::Rule4,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 32, 242, 188, 38},
									DataFieldCategory::Rule5,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 274, 50, 188, 38},
									DataFieldCategory::Rule6,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 274, 98, 188, 38},
									DataFieldCategory::Rule7,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 274, 146, 188, 38},
									DataFieldCategory::Rule8,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 274, 194, 188, 38},
									DataFieldCategory::Rule9,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ 274, 242, 188, 38},
									DataFieldCategory::Rule10,
									FieldType::Text
								}
							},
							10
						};
					case AppearanceType::Bulletin:
						return DocLayout{
							{
								DataLayout{
									Rectangle{38, 92, 64, 78},
									DataFieldCategory::CriminalPhoto1,
									FieldType::Image
								},
								DataLayout{
									Rectangle{38, 190, 64, 78},
									DataFieldCategory::CriminalPhoto2,
									FieldType::Image
								},
								DataLayout{
									Rectangle{38, 288, 64, 78},
									DataFieldCategory::CriminalPhoto3,
									FieldType::Image
								}
							},
							3
						};
					case AppearanceType::Transcript:
						return DocLayout{
							{
								DataLayout{
									Rectangle{14, 32, 272, 362},
									DataFieldCategory::TranscriptPage,
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
							Rectangle{ 8, 418, 56, 12 },
							DataFieldCategory::BoothDate,
							FieldType::Text
						},
						DataLayout{
							Rectangle{ 168, 416, 22, 12 },
							DataFieldCategory::BoothCounter,
							FieldType::Text
						},
						DataLayout{
							Rectangle{ 302, 390, 44, 12 },
							DataFieldCategory::Weight,
							FieldType::Text
						},
					},
					3
				};
			}

#pragma endregion

		}  // namespace v2
	}  // namespace documents
}  // namespace paplease