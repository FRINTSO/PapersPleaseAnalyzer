#include "pch.h"
#include "paplease/analysis/doc_analyzer.h"

#include "paplease/analysis/data/rules.h"
#include "paplease/utils/strfuncs.h"
#include "paplease/documents/data/date.h"

namespace paplease {
    namespace analysis {

        bool DocAnalyzer::AnalyzeDocument(documents::DocType documentType)
        {
            throw;
        }

#if 0

        bool DocAnalyzer::AnalyzeDocumentData(documents::DocType documentType)
        {
            if (documents::IsApplicantDocument(documentType))
            {
                return AnalyzeEntrantDocument(documentType);
            }
            else if (documentType == documents::DocType::RuleBook)
            {
                return AnalyzeRuleBook();
            }
            else if (documentType == documents::DocType::Transcript)
            {
                return AnalyzeTranscript();
            }
            else
            {
                LOG_ERR(
                    "Invalid doctype to analyze: {}",
                    magic_enum::enum_name(documentType)
                );
            }
        }

        bool DocAnalyzer::AnalyzeEntrantDocument(documents::DocType documentType)
        {
            const auto& document = m_store.GetStored<documents::Doc>(documentType);
            const auto& documentData = m_store.GetStored<documents::DocData>(documentType);
            // ANALYSIS PART
            if (!DocValidator(m_context, document, documentData).Validate())
            {
#if !ONE_SHOT
                return true;
#else
                return false;
#endif
            }

            m_store.GetAnalysisStatus(document.GetDocumentType()).wasAnalyzed = true;
            return true;
        }

        bool DocAnalyzer::AnalyzeRuleBook()
        {
            const auto& document = m_store.GetStored<documents::Doc>(documents::DocType::RuleBook);
            auto rulebookOpt = data::CreateRuleBook(document);
            if (!rulebookOpt)
            {
                return false;
            }

            m_context.SetRulebook(std::move(rulebookOpt.value()));
#if DEBUG_LOG_RULES
			LOG_RAW("Rules:");
			for (const auto& rule : m_context.GetRulebook().GetRules())
			{
				LOG("{}", rule.GetDescription());
			}
#endif
            m_store.GetAnalysisStatus(document.GetDocumentType()).wasAnalyzed = true;
			return true;
        }

        bool DocAnalyzer::AnalyzeTranscript()
        {
            const auto& document = m_store.GetStored<documents::Doc>(documents::DocType::Bulletin);
            auto transcriptOpt = data::CreateTranscript(document);
            if (!transcriptOpt)
            {
                return false;
            }

            m_context.SetTranscript(std::move(transcriptOpt.value()));

            m_store.GetAnalysisStatus(document.GetDocumentType()).wasAnalyzed = true;
            return true;
        }

#pragma region DocumentValidator

        bool DocAnalyzer::DocValidator::Validate()
        {
            bool valid = true;

#if DEBUG_LOG_DOCUMENT_DATA
            LOG_RAW(
                "[Document {}]->Data:",
                magic_enum::enum_name(m_document.GetDocumentType())
            );
#endif
            switch (m_document.GetDocumentType())
            {
                case documents::DocType::AccessPermit:
                {
                    valid &= ValidateName();
                    valid &= ValidatePassportNumber();
                    valid &= ValidateExpirationDate();
                    valid &= ValidateForgedOrMissingSeal();
                    valid &= ValidateIssuingCountry();
                    valid &= ValidateHeight();
                    valid &= ValidateWeight();
                    valid &= ValidatePhysicalAppearance();
                    return valid;
                }
                case documents::DocType::CertificateOfVaccination:
                {
                    valid &= ValidateName();
                    valid &= ValidatePassportNumber();
                    valid &= ValidateVaccineExpirationDate();
                    valid &= ValidateMissingVaccine();
                    return valid;
                }
                case documents::DocType::DiplomaticAuthorization:
                {
                    valid &= ValidateName();
                    valid &= ValidateForgedOrMissingSeal();
                    valid &= ValidateAccessToAristotzka();
                    valid &= ValidateIssuingCountry();
                    valid &= ValidatePassportNumber();
                    return valid;
                }
                case documents::DocType::EntryPermit:
                {
                    valid &= ValidateName();
                    valid &= ValidatePassportNumber();
                    valid &= ValidateExpirationDate();
                    valid &= ValidateForgedOrMissingSeal();
                    valid &= ValidatePurpose();
                    return valid;
                }
                case documents::DocType::EntryTicket:
                {
                    valid &= ValidateEntryTicketValidOnDate();
                    return valid;
                }
                case documents::DocType::GrantOfAsylum:
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
                case documents::DocType::IdentityCard:
                {
                    valid &= ValidateName();
                    valid &= ValidateDateOfBirth();
                    valid &= ValidatePhoto();
                    valid &= ValidateDistrict();
                    valid &= ValidateHeight();
                    valid &= ValidateWeight();
                    return valid;
                }
                case documents::DocType::IdentitySupplement:
                {
                    valid &= ValidateHeight();
                    valid &= ValidateWeight();
                    valid &= ValidatePhysicalAppearance();
                    valid &= ValidateThumbprint();
                    valid &= ValidateExpirationDate();
                    return valid;
                }
                case documents::DocType::WorkPass:
                {
                    valid &= ValidateName();
                    valid &= ValidateForgedOrMissingSeal();
                    valid &= ValidateWorkEndDate();
                    return valid;
                }
                case documents::DocType::Passport:
                {
                    valid &= ValidateExpirationDate();
                    valid &= ValidateIssuingCity();
                    valid &= ValidateName();
                    valid &= ValidateDateOfBirth();
                    valid &= ValidatePhoto();
                    valid &= ValidateSex();
                    valid &= ValidatePassportNumber();
                    return valid;
                }
                case documents::DocType::RuleBook:
                case documents::DocType::Bulletin:
                case documents::DocType::Transcript:
                    break;
                case documents::DocType::Invalid:
                    return false;
                default:
                    std::cerr << "DocType not implemented in DocumentValidator::Validate()!\n";
                    return false;
            }
            return false;
        }

        // Against static information, ex. Issuing city, or current date
        bool DocAnalyzer::DocValidator::ValidateExpirationDate() const
        {
            const auto expirationDateData = m_documentData.GetFieldData<documents::FieldCategory::ExpirationDate>();

            // If expiration date data is missing, log the error and return false
            if (!expirationDateData)
            {
                LOG_ERR("Missing expiration date for document '{}'", magic_enum::enum_name(m_document.GetDocumentType()));
                return false;  // Early exit if expiration date is missing
            }

            const auto& expirationDate = expirationDateData->get();

            // Compare expiration date with current date
            bool accepted = m_context.currentDate <= expirationDate;

            if (!accepted)
            {
                LOG_DISCREPANCY("Invalid Expiration Date! Document type: '{}'", magic_enum::enum_name(m_document.GetDocumentType()));
            }
#if DEBUG_LOG_DOCUMENT_DATA
            LOG(
                "[ExpirationDate]={}",
                expirationDateData->get().ToString()
            );
#endif
            return accepted;
        }
        bool DocAnalyzer::DocValidator::ValidateIssuingCity() const
        {
            const auto& city = m_documentData.GetFieldData<documents::FieldCategory::IssuingCity>()->get();
            bool accepted = data::LocationBank::IsValidCity(city);
            if (!accepted)
            {
                LOG_DISCREPANCY(
                    "{} is not a valid city. {}",
                    city,
                    magic_enum::enum_name(m_document.GetDocumentType())
                );
            }
            m_context.SetEntrantCity(data::LocationBank::FromCityString(city));
#if DEBUG_LOG_DOCUMENT_DATA
            LOG(
                "[IssuingCity]={}",
                city
            );
#endif
            return true;
        }
        bool DocAnalyzer::DocValidator::ValidateDistrict() const
        {
            const auto& district = m_documentData.GetFieldData<documents::FieldCategory::District>()->get();
            bool accepted = data::LocationBank::IsValidDistrict(district);
            if (!accepted)
            {
                LOG_DISCREPANCY(
                    "{} is not a valid district. {}",
                    district,
                    magic_enum::enum_name(m_document.GetDocumentType())
                );
            }
            m_context.SetEntrantDistrict(data::LocationBank::FromDistrictString(district));
#if DEBUG_LOG_DOCUMENT_DATA
            LOG(
                "[District]={}",
                district
            );
#endif
            return true;
        }
        bool DocAnalyzer::DocValidator::ValidateForgedOrMissingSeal() const
        {
            bool accepted = m_document.IsAuthentic();
            if (!accepted)
            {
                LOG_DISCREPANCY("Seal missing or forged. '{}'", magic_enum::enum_name(m_document.GetDocumentType()));
            }
            return accepted;
        }
        bool DocAnalyzer::DocValidator::ValidateEntryTicketValidOnDate() const
        {
            const auto expirationDateData = m_documentData.GetFieldData<documents::FieldCategory::ValidDate>();
            assert(expirationDateData.has_value());
            const auto& expirationDate = expirationDateData->get();
            bool accepted = m_context.currentDate == expirationDate;
            if (!accepted)
            {
                LOG_DISCREPANCY("Invalid Enter On Date! '{}'", magic_enum::enum_name(m_document.GetDocumentType()));
            }
#if DEBUG_LOG_DOCUMENT_DATA
            LOG(
                "[ValidOnDate]={}",
                expirationDate.ToString()
            );
#endif
            return accepted;
        }
        bool DocAnalyzer::DocValidator::ValidateIssuingCountry() const
        {
            const auto& country = m_documentData.GetFieldData<documents::FieldCategory::IssuingCountry>()->get();
            bool accepted = data::LocationBank::IsValidCountry(country);
            if (!accepted)
            {
                LOG_DISCREPANCY(
                    "{} is not a valid country. {}",
                    country,
                    magic_enum::enum_name(m_document.GetDocumentType())
                );
            }
            m_context.SetEntrantCountry(data::LocationBank::FromCountryString(country));
#if DEBUG_LOG_DOCUMENT_DATA
            LOG(
                "[IssuingCountry]={}",
                country
            );
#endif
            return true;
        }
        bool DocAnalyzer::DocValidator::ValidateAccessToAristotzka() const
        {
            auto& countryList = m_documentData.GetFieldData<documents::FieldCategory::CountryList>()->get();
            for (auto& countryName : countryList.strs)
            {
                if (countryName == "ARSTOTZKA")
                    return true;
                std::cout << countryName << "\n";
            }
            LOG_DISCREPANCY("Missing access to Arstotzka. '{}'", magic_enum::enum_name(m_document.GetDocumentType()));
            return false;
        }
        bool DocAnalyzer::DocValidator::ValidateMissingVaccine() const
        {
            bool accepted = false;

            // Check if any of the vaccination fields contains "polio"
            const auto vaccine1Opt = m_documentData.GetFieldData<documents::FieldCategory::Vaccination1>();
            const auto vaccine2Opt = m_documentData.GetFieldData<documents::FieldCategory::Vaccination2>();
            const auto vaccine3Opt = m_documentData.GetFieldData<documents::FieldCategory::Vaccination3>();

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
                LOG_DISCREPANCY("Missing polio vaccine. '{}'", magic_enum::enum_name(m_document.GetDocumentType()));
            }
            return accepted;
        }
        bool DocAnalyzer::DocValidator::ValidateVaccineExpirationDate() const
        {
            bool accepted = false;
            documents::data::Date polioExpiryDate;

            // Check each vaccine field explicitly
            const auto vaccine1Opt = m_documentData.GetFieldData<documents::FieldCategory::Vaccination1>();
            const auto vaccine2Opt = m_documentData.GetFieldData<documents::FieldCategory::Vaccination2>();
            const auto vaccine3Opt = m_documentData.GetFieldData<documents::FieldCategory::Vaccination3>();

            // Check if any vaccine data is available and is "polio"
            if (vaccine1Opt && utils::strfuncs::ToLower(vaccine1Opt->get().name) == "polio")
            {
                const auto& vaccineData = vaccine1Opt->get();
                polioExpiryDate = documents::data::Date(vaccineData.date.GetDay(), vaccineData.date.GetMonth(), vaccineData.date.GetYear() + 3);
                accepted = m_context.currentDate <= polioExpiryDate;
            }
            else if (vaccine2Opt && utils::strfuncs::ToLower(vaccine2Opt->get().name) == "polio")
            {
                const auto& vaccineData = vaccine2Opt->get();
                polioExpiryDate = documents::data::Date(vaccineData.date.GetDay(), vaccineData.date.GetMonth(), vaccineData.date.GetYear() + 3);
                accepted = m_context.currentDate <= polioExpiryDate;
            }
            else if (vaccine3Opt && utils::strfuncs::ToLower(vaccine3Opt->get().name) == "polio")
            {
                const auto& vaccineData = vaccine3Opt->get();
                polioExpiryDate = documents::data::Date(vaccineData.date.GetDay(), vaccineData.date.GetMonth(), vaccineData.date.GetYear() + 3);
                accepted = m_context.currentDate <= polioExpiryDate;
            }

            if (!accepted)
            {
                LOG_ERR("Missing or expired polio vaccine. Document type: '{}'", magic_enum::enum_name(m_document.GetDocumentType()));
                LOG_DISCREPANCY("Polio vaccine issue. '{}'", magic_enum::enum_name(m_document.GetDocumentType()));
            }

            return accepted;
        }
        bool DocAnalyzer::DocValidator::ValidatePurpose() const
        {
            const auto purposeOpt = m_documentData.GetFieldData<documents::FieldCategory::Purpose>();
            const auto& purpose = utils::strfuncs::ToLower(purposeOpt->get());

            if (purpose == "work")
            {
                m_context.SetEntrantClassification(data::EntrantClass::Worker);
            }
            else if (purpose == "transit")
            { }
            else if (purpose == "visit")
            { }
            else if (purpose == "immigrate")
            { }
            else
            {
                __debugbreak();
            }

            return true;
        }

        // Against booth
        bool DocAnalyzer::DocValidator::ValidateWeight() const
        {
            // Check if current weight is available
            if (!m_context.currentWeight.has_value())
            {
                // Register reanalysis later, when height is not nullopt
                return false;
            }

            // Get the weight data from the document
            const auto weightDataOpt = m_documentData.GetFieldData<documents::FieldCategory::Weight>();

            // If weight data is unavailable or broken, return false
            if (!weightDataOpt)
            {
                LOG_ERR("Missing or broken weight data for document '{}'", magic_enum::enum_name(m_document.GetDocumentType()));
                return false;
            }

            const auto& weightData = weightDataOpt->get();

            // Compare the current weight with the document's weight
            bool accepted = m_context.currentWeight->value == weightData.value;

            if (!accepted)
            {
                LOG_DISCREPANCY("Invalid weight. Document type: '{}'", magic_enum::enum_name(m_document.GetDocumentType()));
            }
            
#if DEBUG_LOG_DOCUMENT_DATA
            LOG(
                "[Weight]={}",
                m_documentData.GetField(documents::FieldCategory::Weight, false)->get().ToString()
            );
#endif
            return accepted;
        }

        // Against images - not supported yet
        bool DocAnalyzer::DocValidator::ValidateSex() const
        {
            LOG_ERR("DocumentValidator::ValidateSex() Not Implemented!");
            return true;  // Placeholder, assuming valid for now
        }
        bool DocAnalyzer::DocValidator::ValidatePhoto() const
        {
            LOG_ERR("DocumentValidator::ValidatePhoto() Not Implemented!");
            return true;  // Placeholder, assuming valid for now
        }
        bool DocAnalyzer::DocValidator::ValidateHeight() const
        {
            if (!m_context.approximateHeight.has_value())
            {
                // register reanalysis later, when height is not nullopt
                return false;
            }
            int approximateHeight = m_context.approximateHeight->value;
            int exactHeight = m_documentData.GetFieldData<documents::FieldCategory::Height>().value().get().value;

            bool accepted = exactHeight - 5 < approximateHeight && approximateHeight < exactHeight + 5;
            if (!accepted)
            {
                LOG_DISCREPANCY(
                    "Invalid height. Expected '{}', but received '{}'.  '{}'",
                    m_documentData.GetFieldData<documents::FieldCategory::Height>().value().get().value,
                    m_context.approximateHeight->value,
                    magic_enum::enum_name(m_document.GetDocumentType())
                );
            }
#if DEBUG_LOG_DOCUMENT_DATA
            LOG(
                "[Height]={}",
                exactHeight
            );
#endif
            return accepted;
        }
        bool DocAnalyzer::DocValidator::ValidatePhysicalAppearance() const
        {
            LOG_ERR("DocumentValidator::ValidatePhysicalAppearance() Not Implemented!");
#if DEBUG_LOG_DOCUMENT_DATA
            LOG(
                "[PhysicalAppearance]={}",
                m_documentData.GetField(documents::FieldCategory::PhysicalAppearance, false)->get().ToString()
            );
#endif
            return true;  // Placeholder, assuming valid for now
        }
        bool DocAnalyzer::DocValidator::ValidateThumbprint() const
        {
            LOG_ERR("DocumentValidator::ValidateThumbprint() Not Implemented!");

            return true;  // Placeholder, assuming valid for now
        }
        bool DocAnalyzer::DocValidator::ValidateFingerprints() const
        {
            LOG_ERR("DocumentValidator::ValidateFingerprints() Not Implemented!");

            return true;  // Placeholder, assuming valid for now
        }

        // Against other applicant documents
        bool DocAnalyzer::DocValidator::ValidateName() const
        {
            auto nameOpt = m_documentData.GetField(documents::FieldCategory::Name, false);
            if (!nameOpt)
            {
                LOG_ERR("No name data found for document '{}'.", magic_enum::enum_name(m_document.GetDocumentType()));
                return false;  // Return false if the name is missing
            }

            const auto& nameData = nameOpt->get();  // Get the actual field

            bool accepted = m_context.AddToProfile(nameData);
            if (!accepted)
            {
                LOG_DISCREPANCY("Mismatching name '{}' for document '{}'.", nameData.ToString(), magic_enum::enum_name(m_document.GetDocumentType()));
            }
#if DEBUG_LOG_DOCUMENT_DATA
            LOG(
                "[Name]={}",
                nameData.ToString()
            );
#endif
            return accepted;
        }
        bool DocAnalyzer::DocValidator::ValidateDateOfBirth() const
        {
            auto dobOpt = m_documentData.GetField(documents::FieldCategory::DateOfBirth, false);
            if (!dobOpt)
            {
                LOG_ERR("No date of birth data found for document '{}'.", magic_enum::enum_name(m_document.GetDocumentType()));
                return false;
            }

            const auto& dobData = dobOpt->get();

            bool accepted = m_context.AddToProfile(dobData);
            if (!accepted)
            {
                LOG_DISCREPANCY("Mismatching date of birth '{}' for document '{}'.", dobData.ToString(), magic_enum::enum_name(m_document.GetDocumentType()));
            }
#if DEBUG_LOG_DOCUMENT_DATA
            LOG(
                "[DOB]={}",
                dobData.ToString()
            );
#endif
            return accepted;
        }
        bool DocAnalyzer::DocValidator::ValidatePassportNumber() const
        {
            auto passportOpt = m_documentData.GetField(documents::FieldCategory::PassportNumber, false);
            if (!passportOpt)
            {
                LOG_ERR("No passport number data found for document '{}'.", magic_enum::enum_name(m_document.GetDocumentType()));
                return false;  // Return false if the passport number is missing
            }

            const auto& passportData = passportOpt->get();  // Get the actual field
            
            bool accepted = m_context.AddToProfile(passportData);
            if (!accepted)
            {
                LOG_DISCREPANCY("Mismatching passport number '{}' for document '{}'.", passportData.ToString(), magic_enum::enum_name(m_document.GetDocumentType()));
            }
#if DEBUG_LOG_DOCUMENT_DATA
            LOG(
                "[PassportNumber]={}",
                passportData.ToString()
            );
#endif
            return accepted;
        }
        bool DocAnalyzer::DocValidator::ValidateWorkEndDate() const
        {
            LOG_ERR("DocumentValidator::ValidateWorkEndDate() Not Implemented!");
//#if DEBUG_LOG_DOCUMENT_DATA
//            LOG(
//                "[WorkEndDate]={}",
//
//                ),
//#endif
            return true;  // Placeholder, assuming valid for now
        }

#pragma endregion

#endif

    }  // namespace analysis
}  // namespace paplease