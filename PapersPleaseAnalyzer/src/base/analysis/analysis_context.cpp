#include "pch.h"
#include "base/analysis/analysis_context.h"

#include "base/analysis/document_analyzer.h"
#include "base/document_data/date.h"
#include "base/documents/doc_type.h"
#include "base/utils/log.h"

#include <cassert>
#include <utility>

namespace paplease {
	namespace analysis {

		using namespace documents;
		using namespace scannable;

#pragma region DocRegistry

		static inline constexpr bool IsApplicantOrTranscriptDocument(const DocType documentType)
		{
			switch (documentType)
			{
				case DocType::AccessPermit:
				case DocType::CertificateOfVaccination:
				case DocType::DiplomaticAuthorization:
				case DocType::EntryPermit:
				case DocType::EntryTicket:
				case DocType::GrantOfAsylum:
				case DocType::IdentityCard:
				case DocType::IdentitySupplement:
				case DocType::WorkPass:
				case DocType::Passport:
				case DocType::Transcript:
					return true;
				default:
					return false;
			}
		}

		bool DocRegistry::AcquireIfNewDocument(documents::Doc&& document)
		{
			if (!this->IsNewDocument(document)) return false;
			
			LOG("WOW, new {} registered", ToStringView(document.GetDocumentType()));
			
			this->AcquireDocument(std::move(document));
			return true;
		}

		std::optional<DocRegistry::DocRef> DocRegistry::GetDocumentByType(const documents::DocType documentType) const
		{
			for (size_t i = 0; i < m_documentCount; i++)
			{
				if (m_documents[i].GetDocumentType() == documentType)
				{
					return m_documents[i];
				}
			}
			return std::nullopt;
		}

		void DocRegistry::AcquireDocument(documents::Doc&& document)
		{
			assert(m_documentCount < DocRegistry::DocumentCapacity);

			m_documents[m_documentCount] = std::move(document);
			m_documentCount++;
		}

		bool DocRegistry::IsNewDocument(const documents::Doc& document) const
		{
			return !this->HasDocumentOfType(document.GetDocumentType());
		}

		bool DocRegistry::HasDocumentOfType(documents::DocType documentType) const
		{
			for (size_t i = 0; i < m_documentCount; i++)
			{
				if (m_documents[i].GetDocumentType() == documentType)
				{
					return true;
				}
			}
			return false;
		}

		void DocRegistry::OnNewDate()
		{
			for (size_t i = 0; i < m_documentCount; i++)
			{
				m_documents[i] = {};
			}
			m_documentCount = 0;
		}

		void DocRegistry::OnNewApplicant()
		{  // This code should be tested
			for (size_t i = 0; i < m_documentCount; i++)
			{
				if (IsApplicantOrTranscriptDocument(m_documents[i].GetDocumentType()))
				{
					m_documents[i] = {};
				}
			}

			int openSlot = -1;
			for (size_t i = 0; i < m_documentCount; i++)
			{
				if (m_documents[i].GetDocumentType() == DocType::Invalid)
				{
					openSlot = i;
				}
				else
				{
					if (openSlot != -1)
					{
						m_documents[openSlot] = std::move(m_documents[i]);
						openSlot = -1;
					}
				}
			}

			// Scuffed
			m_documentCount = 0;
			for (const auto& document : m_documents)
			{
				if (document.GetDocumentType() != DocType::Invalid)
				{
					m_documentCount++;
				}
			}
		}

#pragma endregion

#pragma region Profile

		void Profile::OnNewDate()
		{
			Clear();
		}

		void Profile::OnNewApplicant()
		{
			Clear();
		}

		void Profile::Clear()
		{
			for (auto& document : m_fields)
			{
				document = {};
			}
			m_fieldCount = 0;
		}

#pragma endregion

#pragma region AnalysisContext

		void AnalysisContext::Update(ScanContext& scanContext)
		{
			BeginLOG("AnalysisContext::Update()");
			// Booth data
			assert(scanContext.boothData.has_value());

			if (this->SetIfNewDate(scanContext.boothData->date))
			{
				LOG("New Date!");
			}

			else if (this->SetIfNewApplicant(scanContext.boothData->applicantNumber))
			{
				LOG("New Applicant!");
			}

			if (scanContext.inspectionData)
			{
				this->HandleInspectionData(*scanContext.inspectionData);
			}

			EndLOG("AnalysisContext::Update()");
		}

		void AnalysisContext::HandleInspectionData(InspectionData& inspectionData)
		{
			for (auto& document : inspectionData.documents)
			{
				// Store document type
				auto documentType = document.GetDocumentType();

				// Store document
				if (documentType == DocType::Invalid || !m_docRegistry.AcquireIfNewDocument(std::move(document)))
				{
					continue;
				}

				this->HandleDocumentByType(documentType);  // Needs to be called after it has been acquired
			}
		}

		void AnalysisContext::HandleDocumentByType(documents::DocType documentType)
		{
			auto document = m_docRegistry.GetDocumentByType(documentType);
			assert(document.has_value());

			switch (documentType)
			{
				case DocType::Bulletin:
				{
					this->StoreBulletin(*document);
					break;
				}
				case DocType::RuleBook:
				{
					this->StoreRuleBook(*document);
					break;
				}
				case DocType::Transcript:
				{
					this->StoreTranscript(*document);
					break;
				}
				default:
				{
					if (IsApplicantDocument(documentType))
					{
						this->AddToProfile(*document);
					}
					else
					{
						LOG_ERR("Unimplemented document '{}' found in NotifyDocumentFound()!", ToStringView(documentType));
					}
					break;
				}
			}
		}

		void AnalysisContext::StoreBulletin(const documents::Doc& document)
		{
			LOG("Criminal data has been loaded!");
			m_criminalData = data::CreateCriminalData(document);
		}

		void AnalysisContext::StoreRuleBook(const documents::Doc& document)
		{
			auto ruleBook = data::CreateRuleBook(document);
			if (!ruleBook)
			{
				LOG_ERR("Rule book could not be loaded!");
				return;
			}
			LOG("Rule book has been loaded!");
			m_ruleBook = ruleBook.value();
		}

		void AnalysisContext::StoreTranscript(const documents::Doc& document)
		{
			LOG("Transcript has been loaded!");
			m_transcript = data::CreateTranscript(document);
		}


		bool AnalysisContext::SetIfNewDate(documents::data::Date date)
		{
			if (date != m_currentDate)
			{
				m_currentDate = date;
				this->OnNewDate();
				return true;
			}
			return false;
		}

		bool AnalysisContext::SetIfNewApplicant(int applicantNumber)
		{
			if (applicantNumber != m_applicantNumber)
			{
				m_applicantNumber = applicantNumber;
				this->OnNewApplicant();
				return true;
			}
			return false;
		}

		void AnalysisContext::OnNewDate()
		{
			m_applicantNumber = 0;

			m_ruleBook = std::nullopt;
			m_criminalData = std::nullopt;
			m_transcript = std::nullopt;

			m_docRegistry.OnNewDate();
			m_profile.OnNewDate();
		}

		void AnalysisContext::OnNewApplicant()
		{
			m_transcript = std::nullopt;

			m_docRegistry.OnNewApplicant();
			m_profile.OnNewApplicant();
		}

#pragma region Profiler

		void AnalysisContext::AddToProfile(const documents::Doc& document)
		{
			auto docData = document.GetDocumentData();

			// Step 1. Validate document against regulations, date, etc
			this->ValidateDocument(document);

			// Step 2. Add each document field to profile, if two entries clash, raise an issue
			//this->CompareAgainstProfile(document);
		}

		void AnalysisContext::ValidateDocument(const documents::Doc& document)
		{
			BeginLOG("AnalysisContext::ValidateDocument({})", ToStringView(document.GetDocumentType()));

			auto validator = DocumentValidator(document);
			std::cout << validator.Validate() << "\n";

			auto docData = document.GetDocumentData();
			auto fields = docData.GetAllValidFields();

			for (const auto& field : fields)
			{
				this->ValidateField(field.value());
			}
			EndLOG("AnalysisContext::ValidateDocument()");
		}

		void AnalysisContext::ValidateField(const documents::FieldData& field)
		{
			switch (field.Category())
			{
				case documents::DataFieldCategory::Nationality:
					LOG("Nationality");
					break;

				case documents::DataFieldCategory::DateOfBirth:
					LOG("DateOfBirth");
					break;

				case documents::DataFieldCategory::Height:
					LOG("Height");
					break;

				case documents::DataFieldCategory::IssuingCity:
					LOG("IssuingCity");
					break;

				case documents::DataFieldCategory::IssuingCountry:
					LOG("IssuingCountry");
					break;

				case documents::DataFieldCategory::Name:
					LOG("Name");
					break;

				case documents::DataFieldCategory::PassportNumber:
					LOG("PassportNumber");
					break;

				case documents::DataFieldCategory::Sex:
					LOG("Sex");
					break;

				case documents::DataFieldCategory::Vaccination1:
					LOG("Vaccination1");
					break;

				case documents::DataFieldCategory::Vaccination2:
					LOG("Vaccination2");
					break;

				case documents::DataFieldCategory::Vaccination3:
					LOG("Vaccination3");
					break;

				case documents::DataFieldCategory::Weight:
					LOG("Weight");
					break;

				default:
					LOG("Unknown category");
					break;

			}
		}

#pragma endregion

#pragma endregion

#pragma region DocumentValidator

		DocumentValidator::DocumentValidator(const documents::Doc& document)
			: m_document{ document }, m_documentData{ document.GetDocumentData() }
		{
			
		}

		bool DocumentValidator::Validate()
		{
			return this->ValidateAuthenticity() && this->ValidateExpiryDate();
		}

		bool DocumentValidator::ValidateAuthenticity() const
		{
			return m_document.IsAuthentic();
		}


		bool DocumentValidator::ValidateExpiryDate() const
		{
			BeginLOG("DocumentValidator::ValidateExpiryDate()");
			std::cout << "Duration: " << m_documentData.Get(DataFieldCategory::Duration).ToText() << "\n";
			std::cout << "EndDate: " << m_documentData.Get(DataFieldCategory::EndDate).ToText() << "\n";
			std::cout << "ExpirationDate: " << m_documentData.Get(DataFieldCategory::ExpirationDate).ToText() << "\n";
			std::cout << "ValidDate: " << m_documentData.Get(DataFieldCategory::ValidDate).ToText() << "\n";
			EndLOG("DocumentValidator::ValidateExpiryDate()");
			return false;
		}

		bool DocumentValidator::ValidateNationality() const
		{
			return false;
		}

		bool DocumentValidator::ValidateDateOfBirth() const
		{
			return false;
		}

		bool DocumentValidator::ValidateVaccineData() const
		{
			return false;
		}

		bool DocumentValidator::ValidateField(const documents::FieldData& field) const
		{
			return false;
		}

#pragma endregion

	}  // namespace analysis
}  // namespace paplease
