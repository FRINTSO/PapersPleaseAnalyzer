#pragma once
#include "pch.h"
#include "base/documents_v2/doc_class.h"

#include <array>
#include <cassert>
#include <iostream>
#include <optional>
#include <type_traits>

#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc.hpp>

#include "base/color.h"
#include "base/common.h"
#include "base/documents_v2/bounding_box_finder.inc"
#include "base/documents_v2/doc_appearance.h"
#include "base/documents_v2/doc_data.h"
#include "base/documents_v2/doc_data_type.h"
#include "base/documents_v2/doc_extractor.h"
#include "base/documents_v2/doc_layout.h"
#include "base/documents_v2/doc_type.h"
#include "base/documents_v2/seal.h"
#include "base/game_view.h"
#include "base/image_process.h"
#include "base/shape.h"

namespace paplease {
	namespace documents {
		namespace v2 {

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

			static constexpr AppearanceType ConvertToAppearanceType(DocType documentType, PassportType passportType)
			{
				if (documentType != DocType::Passport) return static_cast<AppearanceType>(documentType);
				return static_cast<AppearanceType>(passportType);
			}

			AppearanceType Doc::GetAppearanceType() const noexcept
			{
				return ConvertToAppearanceType(m_documentType, m_passportType);
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
#if STRICT_DOCUMENT_SCANNING
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

			static inline constexpr int TryFindBlackPixel(const cv::Mat& mat, int row)
			{
				for (int col = 0; col < mat.cols; col++)
				{
					if (!mat.at<bool>(row, col))
					{ // isBlackPixel
						return col;
					}
				}
				return -1;
			}

			static inline constexpr int CountContinuousBlackPixelsCol(const cv::Mat& mat, int row, int left)
			{
				int count = 0;
				for (int col = left; col < mat.cols; col++)
				{
					if (!mat.at<bool>(row, col))
					{ // isBlackPixel
						count++;
					}
					else
					{
						break;
					}
				}
				return count;
			}

			static inline constexpr int CountContinuousBlackPixelsRow(const cv::Mat& mat, int top, int col)
			{
				int count = 0;
				for (int row = top; row < mat.rows; row++)
				{
					if (!mat.at<bool>(row, col))
					{ // isBlackPixel
						count++;
					}
					else
					{
						break;
					}
				}
				return count;
			}

			static inline constexpr int FindValidRightEdge(const cv::Mat& transcript, int row, int left, int minColLimit)
			{
				int colCount = CountContinuousBlackPixelsCol(transcript, row, left);
				return (colCount >= minColLimit) ? left + colCount - 1 : -1;
			}

			static inline constexpr int FindValidBottomEdge(const cv::Mat& transcript, int top, int right, int minRowLimit)
			{
				int rowCount = CountContinuousBlackPixelsRow(transcript, top, right);
				return (rowCount >= minRowLimit) ? top + rowCount - 1 : -1;
			}

			static void InvertRegion(cv::Mat& transcript, int left, int top, int right, int bottom)
			{
				cv::Mat cutout(transcript, cv::Rect(left, top, right - left + 1, bottom - top + 1));
				cv::bitwise_not(cutout, cutout);
			}

			static inline cv::Mat AdjustBinaryTextColorOfTranscript(const cv::Mat& transcript_bin)
			{
				constexpr auto layout = DocLayout::GetInstant(AppearanceType::Transcript);
				constexpr auto box = layout.GetLayout(DataFieldCategory::TranscriptPage).GetBox();

				cv::Mat transcript(transcript_bin, cv::Rect(box.x, box.y, box.width, box.height));

				constexpr int minColLimit = 20;
				constexpr int minRowLimit = 15;

				for (int row = 0; row < transcript.rows; row++)
				{
					int left = TryFindBlackPixel(transcript, row);
					if (left == -1) continue;

					int right = FindValidRightEdge(transcript, row, left, minColLimit);
					if (right == -1) continue;

					int bottom = FindValidBottomEdge(transcript, row, right, minRowLimit);
					if (bottom == -1) continue;

					InvertRegion(transcript, left, row, right, bottom);

					row = bottom;
				}

				return transcript_bin;
			}
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
						cv::Mat insetImage(thresh, cv::Rect(DOWNSCALE(6), DOWNSCALE(20), DOWNSCALE(238), DOWNSCALE(14)));
						cv::bitwise_not(insetImage, insetImage);
						return thresh;
					}
					case AppearanceType::Passport_Obristan:
					{
						cv::Mat insetImage(grayscale, cv::Rect(DOWNSCALE(16), DOWNSCALE(214), DOWNSCALE(140), DOWNSCALE((96))));
						cv::bitwise_not(insetImage, insetImage);
						cv::Mat thresh = applyThreshold(90);
						return thresh;
					}
					case AppearanceType::RuleBook:
						return applyThreshold(157);
					case AppearanceType::Transcript:
						return AdjustBinaryTextColorOfTranscript(applyThreshold(127));
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
				const auto& layout = this->GetLayout();
				auto layouts = layout.GetAllLayouts();

				DocDataBuilder builder{};

				for (size_t i = 0; i < layout.GetLayoutCount(); i++)
				{
					switch (layouts[i].GetType())
					{
						case FieldType::Text:
						{
							auto raw_text_data = GetFieldString(ExtractDocumentField(binary, layouts[i].GetBox()), m_documentType);
							builder.AddFieldData(layouts[i].GetCategory(), FieldData{ Data{raw_text_data}, layouts[i].GetType(), layouts[i].GetCategory() });
							break;
						}
						case FieldType::Image:
						case FieldType::Invalid:
						default:
						{

							builder.AddFieldData(layouts[i].GetCategory(), FieldData{ Data{} , layouts[i].GetType(), layouts[i].GetCategory() });
							break;
						}
					}
				}

				auto data = builder.GetDocData();
				// assert(data.has_value());
				if (!data.has_value())
				{
					return { };
				}
				return { data.value() };
			}

			DocData Doc::GetDocumentData() const
			{
				assert(this->IsValid());
				return this->ExtractDocData();
			}

#pragma endregion

#pragma region Static Functions

			static consteval std::array<RgbColor, 7> GetPassportColors()
			{
				return std::array<RgbColor, 7>{
					DocAppearance::GetInstant(AppearanceType::Passport_Antegria).GetColors()[0],
						DocAppearance::GetInstant(AppearanceType::Passport_Arstotzka).GetColors()[0],
						DocAppearance::GetInstant(AppearanceType::Passport_Impor).GetColors()[0],
						DocAppearance::GetInstant(AppearanceType::Passport_Kolechia).GetColors()[0],
						DocAppearance::GetInstant(AppearanceType::Passport_Obristan).GetColors()[0],
						DocAppearance::GetInstant(AppearanceType::Passport_Republia).GetColors()[0],
						DocAppearance::GetInstant(AppearanceType::Passport_UnitedFederation).GetColors()[0],
				};
			}

			static PassportType FindPassportType(const GameView& gameView)
			{
				static constexpr auto passportTypesColor = GetPassportColors();

				for (int row = 0; row < gameView.inspection.rows; row++)
				{
					for (int col = 0; col < gameView.inspection.cols; col++)
					{
						for (int i = 0; i < passportTypesColor.size(); i++)
						{
							if (RGB_VAL(passportTypesColor[i]) == BGR_VAL(*gameView.inspection.ptr<BgrColor>(row, col)))
							{
								return static_cast<PassportType>(static_cast<int>(PassportType::Antegria) + i);
							}
						}
					}
				}
				return PassportType::Invalid;
			}

#pragma endregion

#pragma region Factory Functions

			std::optional<Doc> FindDocument(const GameView& gameView, const DocType documentType)
			{
				if (documentType == DocType::Invalid) return std::nullopt;

				PassportType passportType = PassportType::Invalid;

				if (documentType == DocType::Passport)
				{
					passportType = FindPassportType(gameView);
				}

				auto appearanceType = ConvertToAppearanceType(documentType, passportType);
				auto appearance = DocAppearance::Get(appearanceType);

				auto boundingBox = FindDocumentBoundingBox(gameView, appearance);
				if (boundingBox.Empty()) return std::nullopt;

				auto mat = ExtractDocument(gameView.inspection, boundingBox, appearance);
				if (!mat) return std::nullopt;

				return Doc{ std::move(mat.value()), documentType, passportType };
			}

#pragma endregion

		}  // namespace v2
	}  // namespace documents
}  // namespace paplease
