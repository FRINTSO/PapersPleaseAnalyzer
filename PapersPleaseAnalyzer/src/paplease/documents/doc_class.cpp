#pragma once
#include "pch.h"
#include "paplease/documents/doc_class.h"

#include "paplease/common/common.h"
#include "paplease/common/image_process.h"
#include "paplease/common/shape.h"
#include "paplease/core/resource_manager.h"
#include "paplease/documents/doc_appearance.h"
#include "paplease/documents/doc_data.h"
#include "paplease/documents/doc_data_type.h"
#include "paplease/documents/doc_layout.h"
#include "paplease/documents/doc_type.h"
#include "paplease/documents/seal.h"

#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>

#include <cassert>
#include <iostream>
#include <type_traits>

#include "test/documents/test_hsv.h"

namespace paplease {
	namespace documents {

#pragma region Constructors

		Doc::Doc()
			: m_mat{},
			m_documentType{ DocType::Invalid },
			m_passportType{ PassportType::Invalid }
		{}

		Doc::Doc(cv::Mat&& mat, const DocType documentType, const PassportType passportType)
			: m_mat{ std::move(mat) },
			m_documentType{ documentType },
			m_passportType{ passportType }
		{}

#pragma endregion

#pragma region Rule Of Five

		Doc::~Doc()
		{
			m_mat.release();
		}

		Doc::Doc(const Doc& other)
			: Doc{ other.m_mat.clone(), other.m_documentType, other.m_passportType }
		{}

		Doc::Doc(Doc&& other) noexcept
			: m_mat{ std::move(other.m_mat) }, m_documentType{ other.m_documentType }, m_passportType{ other.m_passportType }
		{
			other.m_documentType = DocType::Invalid;
			other.m_passportType = PassportType::Invalid;
		}

		Doc& Doc::operator=(const Doc& other)
		{
			if (this == &other) return *this;

			m_mat = other.m_mat;
			m_documentType = other.m_documentType;
			m_passportType = other.m_passportType;

			return *this;
		}

		Doc& Doc::operator=(Doc&& other) noexcept
		{
			if (this != &other)
			{
				m_mat = std::move(other.m_mat);
				m_documentType = other.m_documentType;
				m_passportType = other.m_passportType;
				other.m_documentType = DocType::Invalid;
				other.m_passportType = PassportType::Invalid;
			}
			return *this;
		}

#pragma endregion

#pragma region Document Appearance And Layout

		const AppearanceType Doc::GetAppearanceType() const noexcept
		{
			return ToAppearanceType(m_documentType, m_passportType);
		}

		const DocAppearance& Doc::GetAppearance() const noexcept
		{
			return DocAppearance::GetRef(this->GetAppearanceType());
		}

		const DocLayout& Doc::GetLayout() const noexcept
		{
			return DocLayout::GetRef(this->GetAppearanceType());
		}

#pragma endregion

#pragma region Getters

		const DocType Doc::GetDocumentType() const
		{
			return m_documentType;
		}

		const PassportType Doc::GetPassportType() const
		{
			return m_passportType;
		}

#pragma endregion

#pragma region Validation

		bool Doc::IsValid() const
		{
			if (m_mat.empty()) return false;

			const Shape shape = GetAppearance().GetShape();
#if ENABLE_STRICT_DOCUMENT_SCANNING
			if (shape.width != m_mat.cols) return false;
			if (shape.height != m_mat.rows) return false;
#else
			//if (shape.width < m_mat.cols) return false;
			//if (shape.height < m_mat.rows) return false;
#endif
			return true;
		}

		bool Doc::HasSeal() const
		{
			switch (m_documentType)
			{
				case DocType::AccessPermit:
				case DocType::DiplomaticAuthorization:
				case DocType::EntryPermit:
				case DocType::GrantOfAsylum:
				case DocType::WorkPass:
					return true;
				default:
					return false;
			}
		}

		bool Doc::IsAuthentic() const
		{
			if (!this->HasSeal()) return true;
			return IsDocumentValidlySealed(m_mat, m_documentType);
		}

#pragma endregion

#pragma region Data Extraction

		
		cv::Mat Doc::PreprocessDocument() const
		{
			// if (m_documentType == DocType::Invalid || !this->IsValid()) return {};
			// if (m_documentType == DocType::Invalid || this->m_mat.empty()) return {};
			assert(m_documentType != DocType::Invalid && !this->m_mat.empty()); // Since FindDocument won't return an invalid object, this shouldn't fail

			cv::Mat grayscale = ToGrayscale(m_mat);

			auto applyThreshold = [&](double threshValue, int thresholdType = cv::THRESH_BINARY)
			{
				cv::Mat thresh;
				cv::threshold(grayscale, thresh, threshValue, 255, thresholdType);
				return thresh;
			};

			switch (this->GetAppearanceType())
			{
				case AppearanceType::AccessPermit:
				case AppearanceType::Bulletin:
				case AppearanceType::EntryPermit:
				case AppearanceType::EntryTicket:
				case AppearanceType::GrantOfAsylum:
				case AppearanceType::IdentitySupplement:
				case AppearanceType::Transcript:
				case AppearanceType::WorkPass:
				case AppearanceType::Passport_Antegria:
				case AppearanceType::Passport_Arstotzka:
				case AppearanceType::Passport_Impor:
				case AppearanceType::Passport_Kolechia:
				case AppearanceType::Passport_Republia:
				case AppearanceType::Passport_UnitedFederation:
					return applyThreshold(127);

				case AppearanceType::CertificateOfVaccination:
					return applyThreshold(95);

				case AppearanceType::DiplomaticAuthorization:
					return applyThreshold(128);

				case AppearanceType::IdentityCard:
				{
					cv::Mat thresh = applyThreshold(127);
					cv::Mat insetImage(thresh, ScaledRectangle(6, 20, 238, 14));
					cv::bitwise_not(insetImage, insetImage);
					return thresh;
				}
				case AppearanceType::Passport_Obristan:
				{
					cv::Mat insetImage(grayscale, ScaledRectangle(16, 214, 140, 96));
					cv::bitwise_not(insetImage, insetImage);
					cv::Mat thresh = applyThreshold(90);
					return thresh;
				}
				case AppearanceType::RuleBook:
					return applyThreshold(157);
				
				default:
				{
					std::cerr << "AppearanceType not implemented in Doc::PreprocessDocument()!\n";
					assert(false);
				}
			}
		}

		DocData Doc::ExtractDocData() const
		{
			auto binary = this->PreprocessDocument();
			const auto& layoutProvider = this->GetLayout();
			auto mouseBoxOpt = GetMouseBox(this->m_mat);
			DocDataBuilder builder{};
			for (const auto& layout : layoutProvider.GetLayouts())
			{
				switch (layout->GetType())
				{
					case FieldType::Text:
					{
						// Check if layout box is colliding with mouse
						if (mouseBoxOpt && mouseBoxOpt->Intersects(layout->GetBox()))
						{
							// Mouse intersects, let's assume data to be broken
							builder.AddField(
								Field{
									Data{ "" },
									layout->GetType(),
									layout->GetCategory()
								},
								layout->GetCategory()
							);
							continue;
						}

						auto raw_text_data = GetFieldString(ExtractDocumentField(binary, layout->GetBox()), m_documentType);
#if OPTIMIZE_DOCDATA
						builder.AddField(
							Field{
								Data{ std::move(raw_text_data) },
								layout->GetType(),
								layout->GetCategory()
							},
							layout->GetCategory()
						);
#else
						builder.AddFieldData(
							layout->GetCategory(),
							Field{
								Data{ std::move(raw_text_data) },
								layout->GetType(),
								layout->GetCategory()
							}
						);
#endif
						break;
					}
					case FieldType::Image:
					{
						auto&& image_data = ExtractDocumentField(binary, layout->GetBox());
#if OPTIMIZE_DOCDATA
						builder.AddField(
							Field{
								Data{
									std::move(image_data)
								},
								layout->GetType(),
								layout->GetCategory()
							},
							layout->GetCategory()
						);
#else
						builder.AddFieldData(
							layout->GetCategory(),
							documents::Field{
								documents::Data{
									data::Photo{ std::move(image_data) }
								},
								layout->GetType(),
								layout->GetCategory()
							}
						);
#endif
						break;
					}
					case FieldType::Invalid:
					default:
					{
#if OPTIMIZE_DOCDATA
						builder.AddField(
							Field{
								Data{},
								layout->GetType(),
								layout->GetCategory()
							},
							layout->GetCategory()
						);
#else
						builder.AddFieldData(layout->GetCategory(), Field{ Data{}, layout->GetType(), layout->GetCategory() });
#endif
						break;
					}
				}
			}
#if OPTIMIZE_DOCDATA
			return builder.Build();
#else
			auto data = builder.GetDocData();
			// assert(data.has_value());
			return { data };
#endif
		}

		DocData Doc::GetDocumentData() const
		{
			assert(this->IsValid());
			return this->ExtractDocData();
		}

#pragma endregion

	}  // namespace documents
}  // namespace paplease
