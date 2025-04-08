#pragma once

namespace paplease {
	namespace documents {
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
									Rectangle{ DOWNSCALE(30), DOWNSCALE(108), DOWNSCALE(234), DOWNSCALE(16) },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(30), DOWNSCALE(152), DOWNSCALE(112), DOWNSCALE(16) },
									//DataFieldCategory::Nationality,
									DataFieldCategory::IssuingCountry,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(156), DOWNSCALE(152), DOWNSCALE(112), DOWNSCALE(16) },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(30), DOWNSCALE(196), DOWNSCALE(112), DOWNSCALE(16) },
									DataFieldCategory::Purpose,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(156), DOWNSCALE(196), DOWNSCALE(112), DOWNSCALE(16) },
									DataFieldCategory::DurationOfStay,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(30), DOWNSCALE(240), DOWNSCALE(112), DOWNSCALE(16) },
									DataFieldCategory::Height,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(156), DOWNSCALE(240), DOWNSCALE(112), DOWNSCALE(16) },
									DataFieldCategory::Weight,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(30), DOWNSCALE(284), DOWNSCALE(234), DOWNSCALE(16) },
									DataFieldCategory::PhysicalAppearance,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(156), DOWNSCALE(328), DOWNSCALE(108), DOWNSCALE(16) },
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
									Rectangle{ DOWNSCALE(32), DOWNSCALE(102), DOWNSCALE(206), DOWNSCALE(12) },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(54), DOWNSCALE(128), DOWNSCALE(184), DOWNSCALE(12) },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(34), DOWNSCALE(190), DOWNSCALE(200), DOWNSCALE(12) },
									DataFieldCategory::Vaccination1,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(34), DOWNSCALE(214), DOWNSCALE(200), DOWNSCALE(12) },
									DataFieldCategory::Vaccination2,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(34), DOWNSCALE(238), DOWNSCALE(200), DOWNSCALE(12) },
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
									Rectangle{ DOWNSCALE(74), DOWNSCALE(182), DOWNSCALE(214), DOWNSCALE(12) },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(98), DOWNSCALE(206), DOWNSCALE(190), DOWNSCALE(12) },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(68), DOWNSCALE(8), DOWNSCALE(140), DOWNSCALE(12) },
									DataFieldCategory::IssuingCountry,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(42), DOWNSCALE(304), DOWNSCALE(240), DOWNSCALE(56) },
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
									Rectangle{ DOWNSCALE(30), DOWNSCALE(182), DOWNSCALE(238), DOWNSCALE(12) },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(30), DOWNSCALE(248), DOWNSCALE(238), DOWNSCALE(12) },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(102), DOWNSCALE(280), DOWNSCALE(166), DOWNSCALE(12) },
									DataFieldCategory::Purpose,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(102), DOWNSCALE(310), DOWNSCALE(166), DOWNSCALE(12) },
									DataFieldCategory::DurationOfStay,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(102), DOWNSCALE(340), DOWNSCALE(166), DOWNSCALE(12) },
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
									Rectangle{ DOWNSCALE(176), DOWNSCALE(64), DOWNSCALE(90), DOWNSCALE(16) },
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
									Rectangle{ DOWNSCALE(144), DOWNSCALE(108), DOWNSCALE(154), DOWNSCALE(34) },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(20), DOWNSCALE(100), DOWNSCALE(120), DOWNSCALE(144) },
									DataFieldCategory::Photo,
									FieldType::Image
								},
								DataLayout{
									Rectangle{ DOWNSCALE(182), DOWNSCALE(156), DOWNSCALE(116), DOWNSCALE(16) },
									//DataFieldCategory::Nationality,
									DataFieldCategory::IssuingCountry,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(182), DOWNSCALE(174), DOWNSCALE(116), DOWNSCALE(16) },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(182), DOWNSCALE(192), DOWNSCALE(116), DOWNSCALE(16) },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(182), DOWNSCALE(210), DOWNSCALE(116), DOWNSCALE(16) },
									DataFieldCategory::Height,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(182), DOWNSCALE(228), DOWNSCALE(116), DOWNSCALE(16) },
									DataFieldCategory::Weight,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(22), DOWNSCALE(250), DOWNSCALE(278), DOWNSCALE(66) },
									DataFieldCategory::FingerPrints,
									FieldType::Image
								},
								DataLayout{
									Rectangle{ DOWNSCALE(156), DOWNSCALE(326), DOWNSCALE(100), DOWNSCALE(16) },
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
									Rectangle{ DOWNSCALE(12), DOWNSCALE(22), DOWNSCALE(232), DOWNSCALE(12) },
									DataFieldCategory::District,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(100), DOWNSCALE(40), DOWNSCALE(144), DOWNSCALE(30) },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(130), DOWNSCALE(80), DOWNSCALE(114), DOWNSCALE(12) },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(130), DOWNSCALE(100), DOWNSCALE(114), DOWNSCALE(12) },
									DataFieldCategory::Height,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(130), DOWNSCALE(120), DOWNSCALE(114), DOWNSCALE(12) },
									DataFieldCategory::Weight,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(12), DOWNSCALE(34), DOWNSCALE(78), DOWNSCALE(96) },
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
									Rectangle{ DOWNSCALE(46), DOWNSCALE(64), DOWNSCALE(114), DOWNSCALE(16) },
									DataFieldCategory::Height,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(46), DOWNSCALE(86), DOWNSCALE(114), DOWNSCALE(16) },
									DataFieldCategory::Weight,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(20), DOWNSCALE(132), DOWNSCALE(142), DOWNSCALE(48) },
									// DataFieldCategory::Description,
									DataFieldCategory::PhysicalAppearance,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(52), DOWNSCALE(278), DOWNSCALE(108), DOWNSCALE(16) },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(74), DOWNSCALE(182), DOWNSCALE(88), DOWNSCALE(70) },
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
									Rectangle{ DOWNSCALE(74), DOWNSCALE(134), DOWNSCALE(204), DOWNSCALE(12) },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(74), DOWNSCALE(164), DOWNSCALE(204), DOWNSCALE(12) },
									DataFieldCategory::Field,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(74), DOWNSCALE(194), DOWNSCALE(204), DOWNSCALE(12) },
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
									Rectangle{ DOWNSCALE(50), DOWNSCALE(220), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(50), DOWNSCALE(238), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(16), DOWNSCALE(278), DOWNSCALE(230), DOWNSCALE(16) },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(50), DOWNSCALE(202), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(50), DOWNSCALE(256), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(16), DOWNSCALE(298), DOWNSCALE(230), DOWNSCALE(16) },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(166), DOWNSCALE(176), DOWNSCALE(80), DOWNSCALE(96) },
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
									Rectangle{ DOWNSCALE(136), DOWNSCALE(212), DOWNSCALE(114), DOWNSCALE(16) },
									DataFieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(136), DOWNSCALE(228), DOWNSCALE(114), DOWNSCALE(16) },
									DataFieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(16), DOWNSCALE(176), DOWNSCALE(230), DOWNSCALE(16) },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(136), DOWNSCALE(196), DOWNSCALE(114), DOWNSCALE(16) },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(136), DOWNSCALE(244), DOWNSCALE(114), DOWNSCALE(16) },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(16), DOWNSCALE(296), DOWNSCALE(124), DOWNSCALE(16) },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(16), DOWNSCALE(196), DOWNSCALE(80), DOWNSCALE(96) },
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
									Rectangle{ DOWNSCALE(140), DOWNSCALE(210), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(140), DOWNSCALE(226), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(16), DOWNSCALE(172), DOWNSCALE(230), DOWNSCALE(16) },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(140), DOWNSCALE(194), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(140), DOWNSCALE(242), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(128), DOWNSCALE(292), DOWNSCALE(118), DOWNSCALE(16) },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(18), DOWNSCALE(192), DOWNSCALE(80), DOWNSCALE(96) },
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
									Rectangle{ DOWNSCALE(138), DOWNSCALE(230), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(138), DOWNSCALE(246), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(16), DOWNSCALE(196), DOWNSCALE(230), DOWNSCALE(16) },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(138), DOWNSCALE(214), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(136), DOWNSCALE(262), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(134), DOWNSCALE(296), DOWNSCALE(118), DOWNSCALE(16) },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(16), DOWNSCALE(214), DOWNSCALE(80), DOWNSCALE(96) },
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
									Rectangle{ DOWNSCALE(54), DOWNSCALE(238), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(54), DOWNSCALE(254), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(16), DOWNSCALE(196), DOWNSCALE(230), DOWNSCALE(16) },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(54), DOWNSCALE(222), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(54), DOWNSCALE(270), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(20), DOWNSCALE(296), DOWNSCALE(118), DOWNSCALE(16) },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(168), DOWNSCALE(214), DOWNSCALE(80), DOWNSCALE(96) },
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
									Rectangle{ DOWNSCALE(54), DOWNSCALE(212), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(54), DOWNSCALE(228), DOWNSCALE(114), DOWNSCALE(16) },
									DataFieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(16), DOWNSCALE(174), DOWNSCALE(230), DOWNSCALE(16) },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(54), DOWNSCALE(196), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(54), DOWNSCALE(244), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(136), DOWNSCALE(296), DOWNSCALE(118), DOWNSCALE(16) },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(170), DOWNSCALE(192), DOWNSCALE(80), DOWNSCALE(96) },
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
									Rectangle{ DOWNSCALE(138), DOWNSCALE(228), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::Sex,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(138), DOWNSCALE(244), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::IssuingCity,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(16), DOWNSCALE(196), DOWNSCALE(230), DOWNSCALE(16) },
									DataFieldCategory::Name,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(138), DOWNSCALE(212), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::DateOfBirth,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(138), DOWNSCALE(260), DOWNSCALE(106), DOWNSCALE(16) },
									DataFieldCategory::ExpirationDate,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(134), DOWNSCALE(296), DOWNSCALE(118), DOWNSCALE(16) },
									DataFieldCategory::PassportNumber,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(16), DOWNSCALE(212), DOWNSCALE(80), DOWNSCALE(96) },
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
									Rectangle{ DOWNSCALE(32), DOWNSCALE(50), DOWNSCALE(188), DOWNSCALE(38) },
									DataFieldCategory::Rule1,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(32), DOWNSCALE(98), DOWNSCALE(188), DOWNSCALE(38) },
									DataFieldCategory::Rule2,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(32), DOWNSCALE(146), DOWNSCALE(188), DOWNSCALE(38) },
									DataFieldCategory::Rule3,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(32), DOWNSCALE(194), DOWNSCALE(188), DOWNSCALE(38) },
									DataFieldCategory::Rule4,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(32), DOWNSCALE(242), DOWNSCALE(188), DOWNSCALE(38) },
									DataFieldCategory::Rule5,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(274), DOWNSCALE(50), DOWNSCALE(188), DOWNSCALE(38) },
									DataFieldCategory::Rule6,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(274), DOWNSCALE(98), DOWNSCALE(188), DOWNSCALE(38) },
									DataFieldCategory::Rule7,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(274), DOWNSCALE(146), DOWNSCALE(188), DOWNSCALE(38) },
									DataFieldCategory::Rule8,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(274), DOWNSCALE(194), DOWNSCALE(188), DOWNSCALE(38) },
									DataFieldCategory::Rule9,
									FieldType::Text
								},
								DataLayout{
									Rectangle{ DOWNSCALE(274), DOWNSCALE(242), DOWNSCALE(188), DOWNSCALE(38) },
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
									Rectangle{ DOWNSCALE(38), DOWNSCALE(92), DOWNSCALE(64), DOWNSCALE(78) },
									DataFieldCategory::CriminalPhoto1,
									FieldType::Image
								},
								DataLayout{
									Rectangle{ DOWNSCALE(38), DOWNSCALE(190), DOWNSCALE(64), DOWNSCALE(78) },
									DataFieldCategory::CriminalPhoto2,
									FieldType::Image
								},
								DataLayout{
									Rectangle{ DOWNSCALE(38), DOWNSCALE(288), DOWNSCALE(64), DOWNSCALE(78) },
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
									Rectangle{ DOWNSCALE(14), DOWNSCALE(32), DOWNSCALE(272), DOWNSCALE(362) },
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
							Rectangle{ DOWNSCALE(8), DOWNSCALE(418), DOWNSCALE(56), DOWNSCALE(12) },
							DataFieldCategory::BoothDate,
							FieldType::Text
						},
						DataLayout{
							Rectangle{ DOWNSCALE(168), DOWNSCALE(416), DOWNSCALE(22), DOWNSCALE(12) },
							DataFieldCategory::BoothCounter,
							FieldType::Text
						},
						DataLayout{
							Rectangle{ DOWNSCALE(302), DOWNSCALE(390), DOWNSCALE(44), DOWNSCALE(12) },
							DataFieldCategory::Weight,
							FieldType::Text
						},
					},
					3
				};
			}

#pragma endregion

	}  // namespace documents
}  // namespace paplease
