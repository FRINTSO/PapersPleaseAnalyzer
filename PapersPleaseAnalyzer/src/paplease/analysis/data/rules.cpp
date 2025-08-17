#include "pch.h"
#include "paplease/analysis/data/rules.h"

#include "paplease/utils/enum_range.h"

#include <string>
#include <unordered_map>

namespace paplease {
    namespace analysis {
        namespace data {

            using namespace documents;

            static constinit Rule g_RuleLookup[] = {
                Rule{  // RequireCurrentDocuments
                    ERule::RequireCurrentDocumentsFromEntrant,
                    RuleDescriptor{
                        ERuleAction::Require,
                        ERuleSubject::CurrentDocuments,
                        ERuleTarget::Entrant,
                    }
                },
                Rule{  // RequirePassport
                    ERule::RequirePassportFromEntrant,
                    RuleDescriptor{
                        ERuleAction::RequireDocument,
                        ERuleSubject::Passport,
                        ERuleTarget::Entrant,
                    }
                },
                Rule{  // RequireArstotzkanPassport
                    ERule::RequireArstotzkanPassportFromEntrant,
                    RuleDescriptor{
                        ERuleAction::RequireDocument,
                        ERuleSubject::ArstotzkanPassport,
                        ERuleTarget::Entrant,
                    }
                },
                Rule{  // RequireIdentityCardFromCitizens
                    ERule::RequireIdentityCardFromCitizens,
                    RuleDescriptor{
                        ERuleAction::RequireDocument,
                        ERuleSubject::IdentityCard,
                        ERuleTarget::Citizens,
                    }
                },
                Rule{  // RequireEntryTicketFromForeigners
                    ERule::RequireEntryTicketFromForeigners,
                    RuleDescriptor{
                        ERuleAction::RequireDocument,
                        ERuleSubject::EntryTicket,
                        ERuleTarget::Foreigners,
                    }
                },
                Rule{  // RequireWorkPassFromWorkers
                    ERule::RequireWorkPassFromWorkers,
                    RuleDescriptor{
                        ERuleAction::RequireDocument,
                        ERuleSubject::WorkPass,
                        ERuleTarget::Workers,
                    }
                },
                Rule{  // RequireDiplomaticAuthorizationFromDiplomats
                    ERule::RequireDiplomaticAuthorizationFromDiplomats,
                    RuleDescriptor{
                        ERuleAction::RequireDocument,
                        ERuleSubject::DiplomaticAuthorization,
                        ERuleTarget::Diplomats,
                    }
                },
                Rule{  // RequireIdentitySupplementFromForeigners
                    ERule::RequireIdentitySupplementFromForeigners,
                    RuleDescriptor{
                        ERuleAction::RequireDocument,
                        ERuleSubject::IdentitySupplement,
                        ERuleTarget::Foreigners,
                    }
                },
                Rule{  // RequireGrantFromAsylumSeekers
                    ERule::RequireGrantFromAsylumSeekers,
                    RuleDescriptor{
                        ERuleAction::RequireDocument,
                        ERuleSubject::Grant,
                        ERuleTarget::AsylumSeekers,
                    }
                },
                Rule{  // RequirePolioVaccination
                    ERule::RequirePolioVaccinationFromEntrant,
                    RuleDescriptor{
                        ERuleAction::RequireDocument,
                        ERuleSubject::PolioVaccination,
                        ERuleTarget::Entrant,
                    }
                },
                Rule{  // RequireAccessPermitFromForeigners
                    ERule::RequireAccessPermitFromForeigners,
                    RuleDescriptor{
                        ERuleAction::RequireDocument,
                        ERuleSubject::AccessPermit,
                        ERuleTarget::Foreigners,
                    }
                },
                Rule{  // RequireEntryPermitFromForeigners
                    ERule::RequireEntryPermitFromForeigners,
                    RuleDescriptor{
                        ERuleAction::RequireDocument,
                        ERuleSubject::EntryPermit,
                        ERuleTarget::Foreigners,
                    }
                },
                Rule{  // RequireSearchOfKolechians
                    ERule::RequireSearchOfKolechians,
                    RuleDescriptor{
                        ERuleAction::Require,
                        ERuleSubject::Search,
                        ERuleTarget::Kolechians,
                    }
                },
                Rule{  // ProhibitEntryFromImpor
                    ERule::ProhibitEntryFromImpor,
                    RuleDescriptor{
                        ERuleAction::Prohibit,
                        ERuleSubject::Entry,
                        ERuleTarget::FromImpor,
                    }
                },
                Rule{
                    ERule::ProhibitEntryFromUnitedFederation,
                    RuleDescriptor{
                        ERuleAction::Prohibit,
                        ERuleSubject::Entry,
                        ERuleTarget::FromUnitedFederation,
                    }
                },
                Rule{  // ProhibitWeaponsAndContraband
                    ERule::ProhibitWeaponsAndContrabandFromEntrant,
                    RuleDescriptor{
                        ERuleAction::Prohibit,
                        ERuleSubject::WeaponsAndContraband,
                        ERuleTarget::Entrant,
                    }
                },
                Rule{  // ConfiscateArstotzkanPassportsFromAltanDistrict
                    ERule::ConfiscateArstotzkanPassportsFromAltanDistrict,
                    RuleDescriptor{
                        ERuleAction::Confiscate,
                        ERuleSubject::ArstotzkanPassport,
                        ERuleTarget::FromAltanDistrict,
                    }
                },
                Rule{  // ConfiscateArstotzkanPassportFromEntrant
                    ERule::ConfiscateArstotzkanPassportFromEntrant,
                    RuleDescriptor{
                        ERuleAction::Confiscate,
                        ERuleSubject::ArstotzkanPassport,
                        ERuleTarget::Entrant,
                    }
                },
            };

            static inline constexpr const Rule* FetchRule(ERule rule)
            {
                if (rule == ERule::Invalid || rule == ERule::Count)
                    return nullptr;

                size_t index = static_cast<size_t>(rule);

                return &g_RuleLookup[index];
            }

            static std::string NormalizeRule(std::string_view sv)
            {
                std::string result(sv);
                std::ranges::transform(result, result.begin(), ::tolower);
                return result;
            }

            static const std::unordered_map<std::string_view, ERule> g_StringRuleLookup = {
                { "all documents must be current",                                ERule::RequireCurrentDocumentsFromEntrant },
                { "all kolechians must be searched",                              ERule::RequireSearchOfKolechians },
                { "arstotzkan citizens must have an id card",                     ERule::RequireIdentityCardFromCitizens },
                { "arstotzkan citizens only",                                     ERule::RequireArstotzkanPassportFromEntrant },
                { "asylum seekers must have a grant",                             ERule::RequireGrantFromAsylumSeekers },
                { "citizens must have an id card",                                ERule::RequireIdentityCardFromCitizens },
                { "confiscate all arstotzkan passports",                          ERule::ConfiscateArstotzkanPassportFromEntrant },
                { "confiscate arstotzkan passports belonging to altan residents", ERule::ConfiscateArstotzkanPassportsFromAltanDistrict },
                { "diplomats require authorization",                              ERule::RequireDiplomaticAuthorizationFromDiplomats },
                { "entrant must have a passport",							      ERule::RequirePassportFromEntrant },
                { "entrant must have polio vaccine cert",                         ERule::RequirePolioVaccinationFromEntrant },
                { "foreigners require an access permit",                          ERule::RequireAccessPermitFromForeigners },
                { "foreigners require an entry permit",                           ERule::RequireEntryPermitFromForeigners },
                { "foreigners require an entry ticket",                           ERule::RequireEntryTicketFromForeigners },
                { "foreigners require an id supplement",                          ERule::RequireIdentitySupplementFromForeigners },
                { "no entry from united federation",                              ERule::ProhibitEntryFromUnitedFederation },
                { "no entry from impor",                                          ERule::ProhibitEntryFromImpor },
                { "no weapons or contraband",                                     ERule::ProhibitWeaponsAndContrabandFromEntrant },
                { "workers must have a work pass",                                ERule::RequireWorkPassFromWorkers },
                { "",                                                             ERule::Invalid },  // when rule is absent in book
            };

            static inline ERule GetERuleFromDescription(const std::string_view& ruleDescription)
            {
                auto key = NormalizeRule(ruleDescription);
                auto it = g_StringRuleLookup.find(key);

                if (it == g_StringRuleLookup.end())
                {
                    paplease::LOG_ERR("Invalid rule... '{}'", ruleDescription);
                    return ERule::Invalid;
                }

                return it->second;
            }

            std::optional<RuleBook> CreateRuleBook(const Doc& document)
            {
                if (!document.IsValid())
                {
                    LOG_ERR("Tried to create rule book from invalid document");
                    return std::nullopt;
                }

                auto ruleData = document.GetDocumentData();
                return CreateRuleBook(ruleData);
            }

            std::optional<RuleBook> CreateRuleBook(const documents::DocData& data)
            {
                RuleBook rulebook{};
                for (auto category : utils::enum_range(FieldCategory::Rule1, FieldCategory::Rule10))
                {
                    auto ruleOpt = data.GetField(category, false);
                    if (!ruleOpt)
                    {
                        return std::nullopt;
                    }

                    const auto& rule = ruleOpt.value().get();
                    if (rule.IsEmpty())
                    {
                        continue;
                    }

                    auto eRule = GetERuleFromDescription(rule.ToString());
                    if (eRule == ERule::Invalid)
                    {
                        return std::nullopt;
                    }

                    rulebook.RegisterRule(eRule);
                }

                return rulebook;
            }

            ERule Rule::GetRule() const
            {
                return m_rule;
            }

            std::string_view Rule::GetDescription() const
            {
                for (const auto& [key, value] : g_StringRuleLookup)
                {
                    if (value == m_rule)
                        return key;
                }

                return "";
            }

            const RuleDescriptor& Rule::GetDescriptor() const
            {
                return m_descriptor;
            }

            ERuleAction Rule::GetAction() const
            {
                return m_descriptor.action;
            }

            ERuleSubject Rule::GetSubject() const
            {
                return m_descriptor.subject;
            }

            ERuleTarget Rule::GetTarget() const
            {
                return m_descriptor.target;
            }

            bool Rule::AppliesTo(const EntrantInfo& entrant) const
            {
                const auto& [action, subject, target] = this->GetDescriptor();
                const EntrantClass& entrantClass = entrant.entrantClass;
                
                switch (target)
                {
                    case ERuleTarget::Entrant:
                        return entrantClass.HasFlag(EntrantClass::Entrant);
                    case ERuleTarget::Citizens:
                        return entrantClass.HasFlag(EntrantClass::Citizen);
                    case ERuleTarget::Foreigners:
                        return entrantClass.HasFlag(EntrantClass::Foreigner);
                    case ERuleTarget::Workers:
                        return entrantClass.HasFlag(EntrantClass::Worker);
                    case ERuleTarget::Diplomats:
                        return entrantClass.HasFlag(EntrantClass::Diplomat);
                    case ERuleTarget::AsylumSeekers:
                        return entrantClass.HasFlag(EntrantClass::AsylumSeeker);
                    case ERuleTarget::Kolechians:
                        return entrantClass.HasFlag(EntrantClass::FromKolechia);
                    case ERuleTarget::FromImpor:
                        return entrantClass.HasFlag(EntrantClass::FromImpor);
                    case ERuleTarget::FromUnitedFederation:
                        return entrantClass.HasFlag(EntrantClass::FromUnitedFed);
                    case ERuleTarget::FromAltanDistrict:
                        return entrantClass.HasFlag(EntrantClass::FromAltanDistrict);
                    default:
                        return false;
                }
            }

            EntrantClass Rule::GetTargetEntrantClass() const
            {
                const auto& [action, subject, target] = this->GetDescriptor();
                switch (target)
                {
                    case ERuleTarget::Entrant:
                        return EntrantClass::Entrant;
                    case ERuleTarget::Citizens:
                        return EntrantClass::Citizen;
                    case ERuleTarget::Foreigners:
                        return EntrantClass::Foreigner;
                    case ERuleTarget::Workers:
                        return EntrantClass::Worker;
                    case ERuleTarget::Diplomats:
                        return EntrantClass::Diplomat;
                    case ERuleTarget::AsylumSeekers:
                        return EntrantClass::AsylumSeeker;
                    case ERuleTarget::Kolechians:
                        return EntrantClass::FromKolechia;
                    case ERuleTarget::FromImpor:
                        return EntrantClass::FromImpor;
                    case ERuleTarget::FromUnitedFederation:
                        return EntrantClass::FromUnitedFed;
                    case ERuleTarget::FromAltanDistrict:
                        return EntrantClass::FromAltanDistrict;
                    default:
                    {
                        LOG_ERR("Shouldn't reach this");
                        assert(false);
                    }
                }
            }

            void RuleBook::RegisterRule(ERule rule)
            {
                const auto* fetchedRule = FetchRule(rule);
                if (fetchedRule == nullptr)
                    return;

                m_activeRules.Add(*fetchedRule);
            }

        }  // namespace data
    }  // namespace analysis
}  // namespace paplease
