#include "pch.h"
#include "paplease/analysis/rule_engine.h"

#include "paplease/analysis/data/location_bank.h"

namespace paplease{
    namespace analysis {
        
        void RuleEngine::Initialize(const data::RuleBook& rulebook)
        {
            m_rulebook = &rulebook;
        }

        void RuleEngine::UpdateApplicableRules()
        {
            for (const data::Rule rule : m_rulebook->GetRules())
            {
                const auto& [action, subject, target] = rule.GetDescriptor();
                const data::ERule eRule = rule.GetRule();

                if (m_entrant.GetEntrantInfo().entrantClass.IsTarget(target) && !m_ruleCtx.applicableRules.Contains(eRule))
                {  // the rule applies to the current understanding of whom the entrant is
                    contexts::TrackedRule trackedRule{ rule, contexts::TrackedRule::Status::Unmet };
#if DEBUG_LOG_APPLICABLE_RULES
                    LOG("Applicable rule: {}", trackedRule.rule.GetDescription());
#endif              

                    switch (eRule)
				    {
					    case data::ERule::RequireEntryPermitFromForeigners:
                        {
                            const auto& entrantClass = m_entrant.GetEntrantInfo().entrantClass;
                            if (entrantClass.IsAsylumSeeker() || entrantClass.IsDiplomat())
                            {
                                continue;
                            }
                            break;
                        }
					    case data::ERule::RequireAccessPermitFromForeigners:
					    case data::ERule::RequireArstotzkanPassportFromEntrant:
					    case data::ERule::RequireDiplomaticAuthorizationFromDiplomats:
					    case data::ERule::RequireEntryTicketFromForeigners:
					    case data::ERule::RequireGrantFromAsylumSeekers:
					    case data::ERule::RequireIdentityCardFromCitizens:
					    case data::ERule::RequireIdentitySupplementFromForeigners:
					    case data::ERule::RequirePassportFromEntrant:
					    case data::ERule::RequirePolioVaccinationFromEntrant:
					    case data::ERule::RequireWorkPassFromWorkers:
					    {
						    break;
					    }
                        default:
                        {
                            // Store the rule in the rule context
                            m_ruleCtx.applicableRules.Set(eRule, std::move(trackedRule));
                            continue;
                        }
				    }
                    // Store the rule in the rule context
                    m_ruleCtx.applicableRules.Set(eRule, std::move(trackedRule));

                    DocLookup requiredDocLookup = data::ERuleSubjectToDocType(rule.GetDescriptor().subject);
                    m_requiredDocsTracker.AddRequiredDocument(requiredDocLookup);

                }
            }
        }

        void RuleEngine::ClassifyEntrantBasedOnDocument(documents::AppearanceType appearanceType)
        {
            DocLookup lookupType{ appearanceType };
            if (lookupType.IsPassport())
            {
                data::ECountry country = data::LocationBank::GetCountryFromPassportType(lookupType.passportType);
                m_entrant.GetEntrantInfo().SetNationaility(country);
            }
            else if (lookupType.documentType == documents::DocType::WorkPass)
            {
                m_entrant.GetEntrantInfo().SetEntrantClassification(
                    data::EntrantClass::Worker
                );
            }
            else if (lookupType.documentType == documents::DocType::DiplomaticAuthorization)
            {
                m_entrant.GetEntrantInfo().SetEntrantClassification(
                    data::EntrantClass::Diplomat
                );
            }
            else if (lookupType.documentType == documents::DocType::GrantOfAsylum)
            {
                m_entrant.GetEntrantInfo().SetEntrantClassification(
                    data::EntrantClass::AsylumSeeker
                );
            }
            else
            {
                return;
            }

            this->UpdateApplicableRules();
        }
    }
}
