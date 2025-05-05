#include "pch.h"
#include "paplease/analysis/doc_engine.h"
#include "paplease/documents/seal.h"
#include "paplease/utils/strfuncs.h"

namespace paplease {
    namespace analysis {

        bool DocEngine::IsAnalyzable(const documents::DocView& docView, const GameView& gameView)
        {
            if (docView.viewArea != ViewArea::InspectionView)
            {
                return false;
            }

            documents::Doc doc = docView.ToDocument(gameView);
            if (!doc.IsValid())
                return false;

            documents::DocData data = doc.GetDocumentData();
            if (data.HasBrokenData())
                return false;

            // Set
            RegisterDocData(doc.GetDocumentType(), std::move(data));
            return true;
        }

        void DocEngine::RegisterDocData(documents::DocType documentType, documents::DocData&& data)
        {
            if (m_entrant.GetMemoryStore().data.Contains(documentType))
            {
                LOG_WARN("Overwriting document of same type in DocEngine (IsAnalyzable)!");
            }
            m_entrant.GetMemoryStore().data.Set(documentType, std::move(data));
        }

        DocAnalysisResult DocEngine::RunAnalysis(const documents::DocView& docView, const GameView& gameView)
        {
            documents::DocType documentType = documents::ToDocType(docView.appearanceType);

            if (documents::IsEntrantDocument(documentType))
            {
                this->RunAnalysisOnEntrantDocument(docView, gameView);
            }
            else if (documentType == documents::DocType::RuleBook)
            {
                return DocAnalysisResult::CanBuildRuleBook;
            }
            else if (documentType == documents::DocType::Transcript)
            {
                return DocAnalysisResult::CanBuildTranscript;
            }
            else if (documentType == documents::DocType::Bulletin)
            {
                return DocAnalysisResult::CanBuildCriminalData;
            }

            return DocAnalysisResult::NoAction;
        }

        class DocFieldComparator
        {  // lightweight
        public:
            DocFieldComparator(
                const documents::Field& field,
                documents::DocType docType,
                contexts::GameContext& game,
                contexts::EntrantContext& entrant
            )
                : m_field(field),
                m_documentType(docType),
                m_game(game),
                m_entrant(entrant) {}

            bool Validate() const
            {
                using FC = documents::FieldCategory;
                switch (m_field.Category())
                {
                    case FC::CountryList:       return ValidateCountryList();
                    case FC::District:          return ValidateDistrict();
                    case FC::DurationOfStay:    return ValidateDurationOfStay();
                    case FC::Field:             return ValidateJobField();
                    //case FC::FingerPrints:      return ValidateFingerprints();  - temporary
                    case FC::IssuingCity:       return ValidateIssuingCity();
                    case FC::IssuingCountry:    return ValidateIssuingCountry();
                    case FC::Purpose:           return ValidatePurpose();
                    //case FC::PhysicalAppearance:return ValidatePhysicalAppearance();
                    case FC::ValidDate:         return ValidateEntryTicketValidOnDate();
                    case FC::EndDate:           return ValidateWorkEndDate();
                    case FC::ExpirationDate:    return ValidateExpirationDate();
                    case FC::DateOfBirth:       return ValidateDateOfBirth();
                    case FC::Name:              return ValidateName();
                    case FC::PassportNumber:    return ValidatePassportNumber();
                    //case FC::Sex:               return ValidateSex();
                    //case FC::Photo:             return ValidatePhoto();
                    case FC::ThumbPrint:        return ValidateThumbprint();
                    case FC::Vaccination1:
                    case FC::Vaccination2:
                    case FC::Vaccination3:      return ValidateVaccineExpirationDate(); // grouped
                    case FC::Height:            return ValidateHeight();
                    case FC::Weight:            return ValidateWeight();
                    //case FC::BoothDate:         return ValidateAgainstBoothDate();
                    //case FC::BoothCounter:      return ValidateBoothCounter(); // optional
                    //case FC::Rule1:
                    //case FC::Rule2:
                    //case FC::Rule3:
                    //case FC::Rule4:
                    //case FC::Rule5:
                    //case FC::Rule6:
                    //case FC::Rule7:
                    //case FC::Rule8:
                    //case FC::Rule9:
                    //case FC::Rule10:            return ValidateRule(m_field.Category());
                    //case FC::CriminalPhoto1:
                    //case FC::CriminalPhoto2:
                    //case FC::CriminalPhoto3:    return ValidateCriminalPhoto();
                    //case FC::TranscriptPage:    return ValidateTranscriptEntry(); // optional
                    //case FC::Invalid:
                    default:
                    {
                        LOG_WARN("Remember to check '{}' field on '{}'.",
                                 magic_enum::enum_name(m_field.Category()),
                                 magic_enum::enum_name(m_documentType)
                        );
                        return false;
                    }
                }
                return true;
            }
            bool HasPolioVaccine() const
            {
                return utils::strfuncs::ToLower(m_field.GetData().ToText()) == "polio";
            }

        private:
            bool ValidateCountryList() const
            {
                auto& countryList = m_field.GetFieldData<documents::FieldCategory::CountryList>()->get();
                for (auto& countryName : countryList.strs)
                {
                    if (countryName == "ARSTOTZKA")
                        return true;
                    std::cout << countryName << "\n";
                }
                LOG_DISCREPANCY("Missing access to Arstotzka. '{}'", magic_enum::enum_name(m_documentType));
                return false;
            }
            bool ValidateDistrict() const
            {
                const auto& district = m_field.GetFieldData<documents::FieldCategory::District>()->get();
                bool accepted = data::LocationBank::IsValidDistrict(district);
                if (!accepted)
                {
                    LOG_DISCREPANCY(
                        "{} is not a valid district. {}",
                        district,
                        magic_enum::enum_name(m_documentType)
                    );
                }
                m_entrant.GetEntrantInfo().SetEntrantDistrict(data::LocationBank::FromDistrictString(district));
#if DEBUG_LOG_DOCUMENT_DATA
                LOG(
                    "[District]={}",
                    district
                );
#endif
                return true;
            }
            bool ValidateDurationOfStay() const
            {
                const auto& duration = m_field.GetFieldData<documents::FieldCategory::DurationOfStay>()->get();

#if DEBUG_LOG_DOCUMENT_DATA
                LOG(
                    "[Duration]={}",
                    duration.c_str()
                );
#endif
                return false;
            }
            bool ValidateJobField() const
            {
                return true;  // Job field can't be invalid
            }
            bool ValidateFingerprints() const
            {
                return false;  // not supported
            }
            bool ValidateIssuingCity() const
            {
                const auto& city = m_field.GetFieldData<documents::FieldCategory::IssuingCity>()->get();
                bool accepted = data::LocationBank::IsValidCity(city);
                if (!accepted)
                {
                    LOG_DISCREPANCY(
                        "{} is not a valid city. {}",
                        city,
                        magic_enum::enum_name(m_documentType)
                    );
                }
                m_entrant.GetEntrantInfo().SetEntrantCity(data::LocationBank::FromCityString(city));
#if DEBUG_LOG_DOCUMENT_DATA
                LOG(
                    "[IssuingCity]={}",
                    city
                );
#endif
                return true;
            }
            bool ValidateIssuingCountry() const
            {
                const auto& country = m_field.GetFieldData<documents::FieldCategory::IssuingCountry>()->get();
                bool accepted = data::LocationBank::IsValidCountry(country);
                if (!accepted)
                {
                    LOG_DISCREPANCY(
                        "{} is not a valid country. {}",
                        country,
                        magic_enum::enum_name(m_documentType)
                    );
                }
                m_entrant.GetEntrantInfo().SetNationaility(data::LocationBank::FromCountryString(country));
#if DEBUG_LOG_DOCUMENT_DATA
                LOG(
                    "[IssuingCountry]={}",
                    country
                );
#endif
                return true;
            }
            bool ValidatePurpose() const
            {
                const auto purposeOpt = m_field.GetFieldData<documents::FieldCategory::Purpose>();
                const auto& purpose = utils::strfuncs::ToLower(purposeOpt->get());

                if (purpose == "work")
                {
                    m_entrant.GetEntrantInfo().SetEntrantClassification(data::EntrantClass::Worker);
                }
                else if (purpose == "transit")
                {
                }
                else if (purpose == "visit")
                {
                }
                else if (purpose == "immigrate")
                {
                }
                else
                {
                    __debugbreak();
                }
#if DEBUG_LOG_DOCUMENT_DATA
                LOG(
                    "[Purpose]={}",
                    purposeOpt->get().c_str()
                );
#endif
                return true;
            }
            bool ValidatePhysicalAppearance() const
            {
                return false; // not supported
            }
            bool ValidateEntryTicketValidOnDate() const
            {
                const auto expirationDateData = m_field.GetFieldData<documents::FieldCategory::ValidDate>();
                assert(expirationDateData.has_value());
                const auto& expirationDate = expirationDateData->get();
                bool accepted = m_game.GetCurrentDate() == expirationDate;
                if (!accepted)
                {
                    LOG_DISCREPANCY("Invalid Enter On Date! '{}'", magic_enum::enum_name(m_documentType));
                }
#if DEBUG_LOG_DOCUMENT_DATA
                LOG(
                    "[ValidOnDate]={}",
                    expirationDate.ToString()
                );
#endif
                return accepted;
            }
            bool ValidateWorkEndDate() const
            {
                return false;  // not yet supported
            }
            bool ValidateExpirationDate() const
            {
                const auto expirationDateData = m_field.GetFieldData<documents::FieldCategory::ExpirationDate>();

                // If expiration date data is missing, log the error and return false
                if (!expirationDateData)
                {
                    LOG_ERR("Missing expiration date for document '{}'", magic_enum::enum_name(m_documentType));
                    return false;  // Early exit if expiration date is missing
                }

                const auto& expirationDate = expirationDateData->get();

                // Compare expiration date with current date
                bool accepted = m_game.GetCurrentDate() <= expirationDate;

                if (!accepted)
                {
                    LOG_DISCREPANCY("Invalid Expiration Date! Document type: '{}'", magic_enum::enum_name(m_documentType));
                }
#if DEBUG_LOG_DOCUMENT_DATA
                LOG(
                    "[ExpirationDate]={}",
                    expirationDateData->get().ToString()
                );
#endif
                return accepted;
            }
            bool ValidateDateOfBirth() const
            {
                const auto& dobField = m_field;
                
                bool accepted = m_entrant.GetProfile().RegisterData(dobField);
                if (!accepted)
                {
                    LOG_DISCREPANCY("Mismatching date of birth '{}' for document '{}'.",
                                    dobField.ToString(),
                                    magic_enum::enum_name(m_documentType)
                    );
                }
#if DEBUG_LOG_DOCUMENT_DATA
                LOG(
                    "[DOB]={}",
                    dobField.ToString()
                );
#endif
                return accepted;
            }
            bool ValidateName() const
            {
                const auto& nameData = m_field;

                bool accepted = m_entrant.GetProfile().RegisterData(nameData);
                if (!accepted)
                {
                    LOG_DISCREPANCY("Mismatching name '{}' for document '{}'.",
                                    nameData.ToString(),
                                    magic_enum::enum_name(m_documentType)
                    );
                }
#if DEBUG_LOG_DOCUMENT_DATA
                LOG(
                    "[Name]={}",
                    nameData.ToString()
                );
#endif
                return accepted;
            }
            bool ValidatePassportNumber() const
            {
                const auto& passportData = m_field;

                bool accepted = m_entrant.GetProfile().RegisterData(passportData);
                if (!accepted)
                {
                    LOG_DISCREPANCY("Mismatching passport number '{}' for document '{}'.",
                                    passportData.ToString(),
                                   magic_enum::enum_name(m_documentType)
                    );
                }
#if DEBUG_LOG_DOCUMENT_DATA
                LOG(
                    "[PassportNumber]={}",
                    passportData.ToString()
                );
#endif
                return accepted;
            }
            bool ValidateSex() const
            {
                return false;  // unsupported
            }
            bool ValidatePhoto() const
            {
                return false;  // unsupported
            }
            bool ValidateThumbprint() const
            {
                return false;  // unsupported
            }
            bool ValidateVaccineExpirationDate() const
            {
                const auto& vaccineData = m_field.GetData().Get<documents::data::Vaccine>();
                auto polioExpiryDate = documents::data::Date(vaccineData.date.GetDay(), vaccineData.date.GetMonth(), vaccineData.date.GetYear() + 3);
                bool accepted = m_game.GetCurrentDate() <= polioExpiryDate;

                if (!accepted)
                {
                    LOG_ERR("Missing or expired polio vaccine. Document type: '{}'", magic_enum::enum_name(m_documentType));
                    LOG_DISCREPANCY("Polio vaccine issue. '{}'", magic_enum::enum_name(m_documentType));
                }

                return accepted;
            }
            bool ValidateHeight() const
            {
                
                if (!m_entrant.GetApproximateHeight().has_value())
                {
                    // register reanalysis later, when height is not nullopt
                    return false;
                }
                int approximateHeight = m_entrant.GetApproximateHeight()->value;
                const int exactHeight = m_field.GetFieldData<documents::FieldCategory::Height>().value().get().value;

                bool accepted = exactHeight - 5 < approximateHeight && approximateHeight < exactHeight + 5;
                if (!accepted)
                {
                    LOG_DISCREPANCY(
                        "Invalid height. Expected '{}', but received '{}'.  '{}'",
                        exactHeight,
                        approximateHeight,
                        magic_enum::enum_name(m_documentType)
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
            bool ValidateWeight() const
            {
                // Check if current weight is available
                if (!m_entrant.GetWeight().has_value())
                {
                    // Register reanalysis later, when height is not nullopt
                    return false;
                }

                // Get the weight data from the document
                const auto weightDataOpt = m_field.GetFieldData<documents::FieldCategory::Weight>();

                // If weight data is unavailable or broken, return false
                if (!weightDataOpt)
                {
                    LOG_ERR("Missing or broken weight data for document '{}'", magic_enum::enum_name(m_documentType));
                    return false;
                }

                const auto& weightData = weightDataOpt->get();

                // Compare the current weight with the document's weight
                bool accepted = m_entrant.GetWeight()->value == weightData.value;

                if (!accepted)
                {
                    LOG_DISCREPANCY("Invalid weight. Document type: '{}'", magic_enum::enum_name(m_documentType));
                }

#if DEBUG_LOG_DOCUMENT_DATA
                LOG(
                    "[Weight]={}{}",
                    weightData.value,
                    magic_enum::enum_name(weightData.unit)
                );
#endif
                return accepted;
            }

        private:
            const documents::Field& m_field;
            documents::DocType m_documentType;
            contexts::GameContext& m_game;
            contexts::EntrantContext& m_entrant;
        };

        void DocEngine::RunAnalysisOnEntrantDocument(const documents::DocView& docView, const GameView& gameView)
        {  // Normal user document

            // Document specific analysis
            // Check seal
            auto documentType = documents::ToDocType(docView.appearanceType);
            bool isSealed = docView.ToDocument(gameView).IsAuthentic();
            if (!isSealed)
            {
                LOG_DISCREPANCY("Seal missing or forged. '{}'", magic_enum::enum_name(documentType));
            }

            // Field analysis
            bool hasVaccineField = false;
            bool hasPolioVaccine = false;
            const auto& data = m_entrant.GetMemoryStore().data.Get(documentType);
            for (const auto& field : data.GetFields())
            {
                DocFieldComparator comparator{ field, documentType, m_game, m_entrant };
                bool validField;

                if (static_cast<size_t>(field.Category()) - static_cast<size_t>(documents::FieldCategory::Vaccination1) < 3)
                {  // is vaccination 1, 2, or 3
                    hasVaccineField = true;
                    if (comparator.HasPolioVaccine())
                    {
                        validField = comparator.Validate();
                        hasPolioVaccine = true;
                    }
                    else
                    {
                        validField = true;
                    }
                }
                else
                {
                    validField = comparator.Validate();
                }

                if (!validField)
                {
                    //LOG_DISCREPANCY("Invalid field: {}", magic_enum::enum_name(field.Category()));
                }
            }

            if (hasVaccineField && !hasPolioVaccine)
            {
                LOG_DISCREPANCY("Missing polio vaccine. '{}'", magic_enum::enum_name(documentType));
            }

        }

        std::optional<data::RuleBook>     DocEngine::TryBuildRuleBook()
        {
            if (!m_entrant.GetMemoryStore().data.Contains(documents::DocType::RuleBook))
            {
                return std::nullopt;
            }
            const auto& data = m_entrant.GetMemoryStore().data.Get(documents::DocType::RuleBook);
            return data::CreateRuleBook(data);
        }

        std::optional<data::Transcript>   DocEngine::TryBuildTranscript()
        {
            return std::nullopt;
        }

        std::optional<data::CriminalData> DocEngine::TryBuildCriminalData()
        {
            return std::nullopt;
        }


        
    }
}