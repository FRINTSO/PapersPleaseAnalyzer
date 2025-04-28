#include "pch.h"
#include "paplease/analysis/analysis_context.h"

#include <magic_enum/magic_enum.hpp>

#include "paplease/documents/data/date.h"
#include "paplease/documents/data/field_data.h"
#include "paplease/documents/doc_type.h"
#include "paplease/utils/log.h"
#include "paplease/utils/strfuncs.h"
#include "paplease/utils/enum_range.h"

#include <cassert>
#include <ios>
#include <utility>

namespace paplease {
	namespace analysis {

		using namespace documents;
		using namespace documents::data;
		using namespace scannable;

#pragma region DocRegistry

		bool DocRegistry::AcquireIfNewDocument(documents::Doc&& document)
		{
			if (!this->IsNewDocument(document)) return false;

			// IS not brokjen
			if (document.GetDocumentData().HasBrokenData())
			{
				LOG_ERR("Document does contain some broken data, needs to rescan. '{}'", magic_enum::enum_name<documents::DocType>(document.GetDocumentType()));
				return false;
			}

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
				if (IsApplicantRelatedDocument(m_documents[i].GetDocumentType()))
				{
					m_documents[i] = {};
				}
			}

			int openSlot = -1;
			for (int i = 0; i < m_documentCount; i++)
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

		bool Profile::RegisterData(const documents::Field& fieldData)
		{
			int index = this->GetFieldIndexByCategoryType(fieldData.Category());
			auto result = this->CompareData(fieldData);
			if (result == MismatchingData)
			{
				return false;
			}
			else if (result == NoData)
			{
				m_fields[index] = fieldData;
			}

			return true;
		}

		int Profile::CompareData(const documents::Field& fieldData) const
		{
			size_t index = this->GetFieldIndexByCategoryType(fieldData.Category());

			if (!this->m_fields[index].has_value())
			{
				return NoData;
			}

			const auto& storedData = this->m_fields[index].value();

			bool isIdenticalData = storedData.GetData() == fieldData.GetData();

			return isIdenticalData ? MatchingData : MismatchingData;
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
			if (scanContext.boothData.date)
			{
				if (this->SetIfNewDate(scanContext.boothData.date.value()))
					LOG_RAW("====================[ New Date ]====================");
			}
			else
			{
				// Log an error if date is missing
				// LOG_ERR("Missing boothData.date! Cannot update the date.");
			}

			if (scanContext.boothData.applicantNumber)
			{
				if (this->SetIfNewApplicant(scanContext.boothData.applicantNumber.value()))
				{
					LOG_RAW("-----------------[ New Applicant ]-----------------");
					// std::cin.get();
					m_entrantInfo = data::EntrantInfo{};
					m_currentWeight = scanContext.boothData.weight;

					if (scanContext.boothData.approximateHeight)
					{
						m_approximateHeight = scanContext.boothData.approximateHeight.value();
					}

					// Rules
					this->ReiterateRulebook();
				}
			}
			else
			{
				// Log an error if applicantNumber is missing
				LOG_ERR("Missing boothData.applicantNumber! Cannot update the applicant.");
			}

			this->SaveScannedDocuments(scanContext.inspectionData, *scanContext.currentGameView);
		}

		//
		// Scan Rules
		// Then build scan-instructions from rules
		// 
		//

		void AnalysisContext::SaveScannedDocuments(InspectionData& inspectionData, const GameView& gameView)
		{
			for (const auto& docView : inspectionData.scannedDocuments)
			{
				// Store document type
				auto documentType = ToDocType(docView.appearanceType);

				// Store document
				if (documentType == DocType::Invalid || !m_docRegistry.AcquireIfNewDocument( docView.ToDocument(gameView) ))
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
						LOG_ERR("Unimplemented document '{}' found in NotifyDocumentFound()!", magic_enum::enum_name<documents::DocType>(documentType));
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

		void AnalysisContext::ReiterateRulebook() const
		{
			if (!this->m_ruleBook)
			{
				LOG("Scan rule book!");
				return;
			}

			// [Entrant type] -> [Required DocType]:
			// Entrant -> Passport
			// ArstotzkanPassport -> Entrant
			// Id Card -> Citizen
			// EntryTicket -> Foreigner
			// WorkPass -> Worker
			// DiplomaticAuth -> Diplomat
			// Id Supplement -> Foreigner
			// Grant -> Asylum Seeker
			// PolioVaccine -> Entrant
			// AccessPermit -> Foreigner
			// EntryPermit -> Foreigner
			//

			for (const auto& rule : m_ruleBook->GetRules())
			{
				LOG("{}", rule.GetDescription());

				//rule->GetDescriptor().
			}


		}

		bool AnalysisContext::SetIfNewDate(documents::data::Date date)
		{
			if (date != m_currentDate)
			{
				this->OnNewDate(date);
				return true;
			}
			return false;
		}

		bool AnalysisContext::SetIfNewApplicant(int applicantNumber)
		{
			if (applicantNumber != m_applicantNumber)
			{
				this->OnNewApplicant(applicantNumber);
				return true;
			}
			return false;
		}

		void AnalysisContext::OnNewDate(documents::data::Date date)
		{
			m_currentDate = date;
			m_currentWeight = std::nullopt;
			m_approximateHeight = std::nullopt;
			m_applicantNumber = 0;

			m_ruleBook = std::nullopt;
			m_criminalData = std::nullopt;
			m_transcript = std::nullopt;

			m_docTracker.OnNewDate();
			m_docRegistry.OnNewDate();
			m_profile.OnNewDate();
		}

		void AnalysisContext::OnNewApplicant(int applicantNumber)
		{
			m_currentWeight = std::nullopt;
			m_approximateHeight = std::nullopt;
			m_applicantNumber = applicantNumber;

			m_transcript = std::nullopt;

			m_docTracker.OnNewApplicant();
			m_docRegistry.OnNewApplicant();
			m_profile.OnNewApplicant();
			m_entrantInfo = data::EntrantInfo();
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
			BeginLOG("AnalysisContext::ValidateDocument({})", magic_enum::enum_name<documents::DocType>(document.GetDocumentType()));

			auto validator = DocumentValidator(document, *this);
			bool result = validator.Validate();
			LOG("Is valid: {}", result);
			
			EndLOG("AnalysisContext::ValidateDocument()");
		}

#pragma endregion

#pragma endregion

#pragma region DocumentValidator

		DocumentValidator::DocumentValidator(const documents::Doc& document, AnalysisContext& analysisContext)
			: m_analysisContext{ analysisContext }, m_document { document}, m_documentData{ document.GetDocumentData() }
		{}

		bool DocumentValidator::Validate()
		{
			bool valid = true;
			
			switch (m_document.GetDocumentType())
			{
				case DocType::AccessPermit:
				{
					valid &= ValidateName();
					valid &= ValidatePassportNumber();
					valid &= ValidateExpirationDate();
					valid &= ValidateForgedOrMissingSeal();
					valid &= ValidateIssuingCountry();
					valid &= ValidateHeight();
					valid &= ValidateWeight();
					valid &= ValidatePhysicalAppearance();
					//valid &= ValidateAgainstTranscript();
					return valid;
				}
				case DocType::CertificateOfVaccination:
				{
					valid &= ValidateName();
					valid &= ValidatePassportNumber();
					valid &= ValidateVaccineExpirationDate();
					valid &= ValidateMissingVaccine();
					return valid;
				}
				case DocType::DiplomaticAuthorization:
				{
					valid &= ValidateName();
					valid &= ValidateForgedOrMissingSeal();
					valid &= ValidateAccessToAristotzka();
					valid &= ValidateIssuingCountry();
					valid &= ValidatePassportNumber();
					return valid;
				}
				case DocType::EntryPermit:
				{
					valid &= ValidateName();
					valid &= ValidatePassportNumber();
					valid &= ValidateExpirationDate();
					valid &= ValidateForgedOrMissingSeal();
					//valid &= ValidateAgainstTranscript();
					return valid;
				}
				case DocType::EntryTicket:
				{
					valid &= ValidateEntryTicketValidOnDate();
					return valid;
				}
				case DocType::GrantOfAsylum:
				{
					valid &= ValidateForgedOrMissingSeal();
					valid &= ValidateName();
					valid &= ValidateIssuingCountry();
					valid &= ValidatePassportNumber();
					valid &= ValidateDateOfBirth();
					valid &= ValidateHeight();
					valid &= ValidateWeight();
					valid &= ValidatePhoto();
					valid &= ValidateFingerprints();
					valid &= ValidateExpirationDate();
					return valid;
				}
				case DocType::IdentityCard:
				{
					valid &= ValidateName();
					valid &= ValidateDateOfBirth();
					valid &= ValidatePhoto();
					valid &= ValidateDistrict();
					valid &= ValidateHeight();
					valid &= ValidateWeight();
					return valid;
				}
				case DocType::IdentitySupplement:
				{
					valid &= ValidateHeight();
					valid &= ValidateWeight();
					valid &= ValidatePhysicalAppearance();
					valid &= ValidateThumbprint();
					valid &= ValidateExpirationDate();
					return valid;
				}
				case DocType::WorkPass:
				{
					valid &= ValidateName();
					valid &= ValidateForgedOrMissingSeal();
					valid &= ValidateWorkEndDate();
					return valid;
				}
				case DocType::Passport:
				{
					m_analysisContext.m_entrantInfo.nationality = data::LocationBank::GetCountryFromPassportType(m_document.GetPassportType());

					//valid &= ValidateAgainstRulebook();
					valid &= ValidateExpirationDate();
					valid &= ValidateIssuingCity();
					valid &= ValidateName();
					valid &= ValidateDateOfBirth();
					valid &= ValidatePhoto();
					valid &= ValidateSex();
					valid &= ValidatePassportNumber();
					return valid;
				}
				case DocType::RuleBook:
				case DocType::Bulletin:
				case DocType::Transcript:
					break;
				case DocType::Invalid:
					return false;
				default:
					std::cerr << "DocType not implemented in DocumentValidator::Validate()!\n";
					return false;
			}
			return false;
		}

		// Against static information, ex. Issuing city, or current date
		bool DocumentValidator::ValidateExpirationDate() const
		{
			const auto expirationDateData = m_documentData.GetFieldData<FieldCategory::ExpirationDate>();

			// If expiration date data is missing, log the error and return false
			if (!expirationDateData)
			{
				LOG_ERR("Missing expiration date for document '{}'", magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType()));
				return false;  // Early exit if expiration date is missing
			}

			const auto& expirationDate = expirationDateData->get();

			// Compare expiration date with current date
			bool accepted = m_analysisContext.m_currentDate <= expirationDate;

			if (!accepted)
			{
				LOG_DISCREPANCY("Invalid Expiration Date! Document type: '{}'", magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType()));
			}

			return accepted;
		}

		bool DocumentValidator::ValidateIssuingCity() const
		{
			const auto& city = m_documentData.GetFieldData<FieldCategory::IssuingCity>()->get();
			bool accepted = data::LocationBank::IsValidCity(city);
			if (!accepted)
			{
				LOG_DISCREPANCY(
					"{} is not a valid city. {}", 
					city,
					magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType())
				);
			}
			m_analysisContext.m_entrantInfo.city = data::LocationBank::FromCityString(city);
			return true;
		}

		bool DocumentValidator::ValidateDistrict() const
		{
			const auto& district = m_documentData.GetFieldData<FieldCategory::District>()->get();
			bool accepted = data::LocationBank::IsValidDistrict(district);
			if (!accepted)
			{
				LOG_DISCREPANCY(
					"{} is not a valid district. {}",
					district,
					magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType())
				);
			}
			m_analysisContext.m_entrantInfo.district = data::LocationBank::FromDistrictString(district);
			return true;
		}

		bool DocumentValidator::ValidateForgedOrMissingSeal() const
		{
			bool accepted = m_document.IsAuthentic();
			if (!accepted)
			{
				LOG_DISCREPANCY("Seal missing or forged. '{}'", magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType()));
			}
			return accepted;
		}

		//bool DocumentValidator::ValidateNationality() const
		//{
		//	bool accepted = m_analysisContext.m_locationBank.IsValidCountry(m_documentData.GetFieldData<DataFieldCategory::Nationality>()->get());
		//	if (!accepted)
		//	{
		//		LOG_DISCREPANCY(
		//			"{} is not a valid country. {}",
		//			m_documentData.GetFieldData<DataFieldCategory::IssuingCountry>()->get(),
		//			magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType())
		//		);
		//	}
		//	return true;
		//}

		bool DocumentValidator::ValidateEntryTicketValidOnDate() const
		{
			const auto expirationDateData = m_documentData.GetFieldData<FieldCategory::ValidDate>();
			assert(expirationDateData.has_value());
			const auto& expirationDate = expirationDateData->get();
			bool accepted = m_analysisContext.m_currentDate == expirationDate;
			if (!accepted)
			{
				LOG_DISCREPANCY("Invalid Enter On Date! '{}'", magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType()));
			}
			return accepted;
		}

		bool DocumentValidator::ValidateIssuingCountry() const
		{
			const auto& country = m_documentData.GetFieldData<FieldCategory::IssuingCountry>()->get();
			bool accepted = data::LocationBank::IsValidCountry(country);
			if (!accepted)
			{
				LOG_DISCREPANCY(
					"{} is not a valid country. {}",
					country,
					magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType())
				);
			}
			// m_analysisContext.m_entrantInfo.nationality = data::LocationBank::FromCountryString(country);
			return true;
		}

		bool DocumentValidator::ValidateAccessToAristotzka() const
		{
			auto& countryList = m_documentData.GetFieldData<FieldCategory::CountryList>()->get();
			for (auto& countryName : countryList.strs)
			{
				if (countryName == "ARSTOTZKA")
					return true;
				std::cout << countryName << "\n";
			}
			LOG_DISCREPANCY("Missing access to Arstotzka. '{}'", magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType()));
			return false;
		}

		bool DocumentValidator::ValidateMissingVaccine() const
		{
			bool accepted = false;

			// Check if any of the vaccination fields contains "polio"
			const auto vaccine1Opt = m_documentData.GetFieldData<FieldCategory::Vaccination1>();
			const auto vaccine2Opt = m_documentData.GetFieldData<FieldCategory::Vaccination2>();
			const auto vaccine3Opt = m_documentData.GetFieldData<FieldCategory::Vaccination3>();

			if (vaccine1Opt && utils::strfuncs::ToLower(vaccine1Opt->get().name) == "polio")
			{
				accepted = true;
			}
			else if (vaccine2Opt && utils::strfuncs::ToLower(vaccine2Opt->get().name) == "polio")
			{
				accepted = true;
			}
			else if (vaccine3Opt && utils::strfuncs::ToLower(vaccine3Opt->get().name) == "polio")
			{
				accepted = true;
			}

			// If not accepted, log the discrepancy
			if (!accepted)
			{
				LOG_DISCREPANCY("Missing polio vaccine. '{}'", magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType()));
			}

			return accepted;
		}

		bool DocumentValidator::ValidateVaccineExpirationDate() const
		{
			bool accepted = false;
			Date polioExpiryDate;

			// Check each vaccine field explicitly
			const auto vaccine1Opt = m_documentData.GetFieldData<FieldCategory::Vaccination1>();
			const auto vaccine2Opt = m_documentData.GetFieldData<FieldCategory::Vaccination2>();
			const auto vaccine3Opt = m_documentData.GetFieldData<FieldCategory::Vaccination3>();

			// Check if any vaccine data is available and is "polio"
			if (vaccine1Opt && utils::strfuncs::ToLower(vaccine1Opt->get().name) == "polio")
			{
				const auto& vaccineData = vaccine1Opt->get();
				polioExpiryDate = Date(vaccineData.date.GetDay(), vaccineData.date.GetMonth(), vaccineData.date.GetYear() + 3);
				accepted = m_analysisContext.m_currentDate <= polioExpiryDate;
			}
			else if (vaccine2Opt && utils::strfuncs::ToLower(vaccine2Opt->get().name) == "polio")
			{
				const auto& vaccineData = vaccine2Opt->get();
				polioExpiryDate = Date(vaccineData.date.GetDay(), vaccineData.date.GetMonth(), vaccineData.date.GetYear() + 3);
				accepted = m_analysisContext.m_currentDate <= polioExpiryDate;
			}
			else if (vaccine3Opt && utils::strfuncs::ToLower(vaccine3Opt->get().name) == "polio")
			{
				const auto& vaccineData = vaccine3Opt->get();
				polioExpiryDate = Date(vaccineData.date.GetDay(), vaccineData.date.GetMonth(), vaccineData.date.GetYear() + 3);
				accepted = m_analysisContext.m_currentDate <= polioExpiryDate;
			}

			if (!accepted)
			{
				LOG_ERR("Missing or expired polio vaccine. Document type: '{}'", magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType()));
				LOG_DISCREPANCY("Polio vaccine issue. '{}'", magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType()));
			}

			return accepted;
		}
		// Against booth
		bool DocumentValidator::ValidateWeight() const
		{
			// Check if current weight is available
			if (!m_analysisContext.m_currentWeight.has_value())
			{
				// Register reanalysis later, when height is not nullopt
				return false;
			}

			// Get the weight data from the document
			const auto weightDataOpt = m_documentData.GetFieldData<FieldCategory::Weight>();

			// If weight data is unavailable or broken, return false
			if (!weightDataOpt)
			{
				LOG_ERR("Missing or broken weight data for document '{}'", magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType()));
				return false;
			}

			const auto& weightData = weightDataOpt->get();

			// Compare the current weight with the document's weight
			bool accepted = m_analysisContext.m_currentWeight->value == weightData.value;

			if (!accepted)
			{
				LOG_DISCREPANCY("Invalid weight. Document type: '{}'", magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType()));
			}

			return accepted;
		}

		// Against images - not supported yet
		bool DocumentValidator::ValidateSex() const
		{
			LOG_ERR("DocumentValidator::ValidateSex() Not Implemented!");
			return true;  // Placeholder, assuming valid for now
		}

		bool DocumentValidator::ValidatePhoto() const
		{
			LOG_ERR("DocumentValidator::ValidatePhoto() Not Implemented!");
			return true;  // Placeholder, assuming valid for now
		}

		bool DocumentValidator::ValidateHeight() const
		{
			if (!m_analysisContext.m_approximateHeight.has_value())
			{
				// register reanalysis later, when height is not nullopt
				return false;
			}
			int approximateHeight = m_analysisContext.m_approximateHeight->value;
			int exactHeight = m_documentData.GetFieldData<FieldCategory::Height>().value().get().value;

			bool accepted = exactHeight - 5 < approximateHeight && approximateHeight < exactHeight + 5;
			if (!accepted)
			{
				LOG_DISCREPANCY(
					"Invalid height. Expected '{}', but received '{}'.  '{}'",
					m_documentData.GetFieldData<FieldCategory::Height>().value().get().value,
					m_analysisContext.m_approximateHeight->value,
					magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType())
				);
			}
			return accepted;
		}

		bool DocumentValidator::ValidatePhysicalAppearance() const
		{
			LOG_ERR("DocumentValidator::ValidatePhysicalAppearance() Not Implemented!");
			return true;  // Placeholder, assuming valid for now
		}

		bool DocumentValidator::ValidateThumbprint() const
		{
			LOG_ERR("DocumentValidator::ValidateThumbprint() Not Implemented!");
			return true;  // Placeholder, assuming valid for now
		}

		bool DocumentValidator::ValidateFingerprints() const
		{
			LOG_ERR("DocumentValidator::ValidateFingerprints() Not Implemented!");
			return true;  // Placeholder, assuming valid for now
		}

		//// Against rulebook, transcript, bulletin
		//bool DocumentValidator::ValidateAgainstRulebook() const
		//{
		//	LOG_ERR("DocumentValidator::ValidateAgainstRulebook() Not Implemented!");
		//	return true;  // Placeholder, assuming valid for now
		//}

		//bool DocumentValidator::ValidateAgainstTranscript() const
		//{
		//	LOG_ERR("DocumentValidator::ValidateAgainstTranscript() Not Implemented!");
		//	return true;  // Placeholder, assuming valid for now
		//}

		// Against other applicant documents
		bool DocumentValidator::ValidateName() const
		{
			auto nameOpt = m_documentData.GetField(FieldCategory::Name, false);
			if (!nameOpt)
			{
				LOG_ERR("No name data found for document '{}'.", magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType()));
				return false;  // Return false if the name is missing
			}

			const auto& nameData = nameOpt->get();  // Get the actual field

			bool accepted = m_analysisContext.m_profile.RegisterData(nameData);
			if (!accepted)
			{
				LOG_DISCREPANCY("Mismatching name '{}' for document '{}'.", nameData.ToString(), magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType()));
			}

			return accepted;
		}

		bool DocumentValidator::ValidateDateOfBirth() const
		{
			auto dobOpt = m_documentData.GetField(FieldCategory::DateOfBirth, false);
			if (!dobOpt)
			{
				LOG_ERR("No date of birth data found for document '{}'.", magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType()));
				return false;
			}

			const auto& dobData = dobOpt->get();

			bool accepted = m_analysisContext.m_profile.RegisterData(dobData);
			if (!accepted)
			{
				LOG_DISCREPANCY("Mismatching date of birth '{}' for document '{}'.", dobData.ToString(), magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType()));
			}

			return accepted;
		}

		bool DocumentValidator::ValidatePassportNumber() const
		{
			auto passportOpt = m_documentData.GetField(FieldCategory::PassportNumber, false);
			if (!passportOpt)
			{
				LOG_ERR("No passport number data found for document '{}'.", magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType()));
				return false;  // Return false if the passport number is missing
			}

			const auto& passportData = passportOpt->get();  // Get the actual field

			bool accepted = m_analysisContext.m_profile.RegisterData(passportData);
			if (!accepted)
			{
				LOG_DISCREPANCY("Mismatching passport number '{}' for document '{}'.", passportData.ToString(), magic_enum::enum_name<documents::DocType>(m_document.GetDocumentType()));
			}

			return accepted;
		}

		bool DocumentValidator::ValidateWorkEndDate() const
		{
			LOG_ERR("DocumentValidator::ValidateWorkEndDate() Not Implemented!");
			return true;  // Placeholder, assuming valid for now
		}


#pragma endregion

	}  // namespace analysis
}  // namespace paplease
