#pragma once
#include <array>
#include <optional>
#include <string>

#include "paplease/documents/doc_class.h"
#include "paplease/core/fixed.h"

namespace paplease {
    namespace analysis {
        namespace data {

            enum class ERule : u8
            {
                Invalid = static_cast<u8>(-1),

                // === REQUIREMENTS ===
                RequireCurrentDocumentsFromEntrant = 0, // If this flag is set, mediator should know what functions to call
                RequirePassportFromEntrant,
                RequireArstotzkanPassportFromEntrant,
                RequireIdentityCardFromCitizens,
                RequireEntryTicketFromForeigners,
                RequireWorkPassFromWorkers,
                RequireDiplomaticAuthorizationFromDiplomats,
                RequireIdentitySupplementFromForeigners,
                RequireGrantFromAsylumSeekers,
                RequirePolioVaccinationFromEntrant,
                RequireAccessPermitFromForeigners,
                RequireEntryPermitFromForeigners,
                RequireSearchOfKolechians,

                // === PROHIBITIONS ===
                ProhibitEntryFromImpor,
                ProhibitEntryFromUnitedFederation,
                ProhibitWeaponsAndContrabandFromEntrant,

                // === CONFISCATIONS ===
                ConfiscateArstotzkanPassportsFromAltanDistrict,
                ConfiscateArstotzkanPassportFromEntrant,

                Count
            };

            // Scan order:
            // All entrants require passport (RequirePassportFromEntrant)
            // 1. Passport belongs to citizen
            //  -> Require id card (RequireIdentityCardFromCitizens)
            //  .
            // 2. Passport belongs to foreigner
            //  -> Require EntryTicket (RequireEntryTicketFromForeigners)
            //  .
            //  -> Require IdentitySupplement (RequireIdentitySupplementFromForeigners)
            //  -> Require AccessPermit (RequireAccessPermitFromForeigners)
            //  -> Require EntryPermit (RequireEntryPermitFromForeigners)
            // 
            //

            enum class ERuleAction : u8
            {
                Invalid = static_cast<u8>(-1),
                Require = 0,    // Need information
                Prohibit,   // Limit
                Confiscate  // Limit
            };

            enum class ERuleSubject : u8
            {
                Invalid = static_cast<u8>(-1),
                CurrentDocuments = 0,
                Passport,
                ArstotzkanPassport,
                IdentityCard,
                EntryTicket,
                WorkPass,
                DiplomaticAuthorization,
                IdentitySupplement,
                Grant,
                PolioVaccination,
                AccessPermit,
                EntryPermit,
                WeaponsAndContraband,
                Search,
                Entry,
            };

            static inline constexpr std::pair<documents::DocType, documents::PassportType> ERuleSubjectToDocType(ERuleSubject subject)
            {
                switch (subject)
                {
                    case ERuleSubject::Passport:
                        return std::make_pair(documents::DocType::Passport, documents::PassportType::Invalid);
                    case ERuleSubject::ArstotzkanPassport:
                        return std::make_pair(documents::DocType::Passport, documents::PassportType::Arstotzka);
                    case ERuleSubject::IdentityCard:
                        return std::make_pair(documents::DocType::IdentityCard, documents::PassportType::Invalid);
                    case ERuleSubject::EntryTicket:
                        return std::make_pair(documents::DocType::EntryTicket, documents::PassportType::Invalid);
                    case ERuleSubject::WorkPass:
                        return std::make_pair(documents::DocType::WorkPass, documents::PassportType::Invalid);
                    case ERuleSubject::DiplomaticAuthorization:
                        return std::make_pair(documents::DocType::DiplomaticAuthorization, documents::PassportType::Invalid);
                    case ERuleSubject::IdentitySupplement:
                        return std::make_pair(documents::DocType::IdentitySupplement, documents::PassportType::Invalid);
                    case ERuleSubject::Grant:
                        return std::make_pair(documents::DocType::GrantOfAsylum, documents::PassportType::Invalid);
                    case ERuleSubject::PolioVaccination:
                        return std::make_pair(documents::DocType::CertificateOfVaccination, documents::PassportType::Invalid);
                    case ERuleSubject::AccessPermit:
                        return std::make_pair(documents::DocType::AccessPermit, documents::PassportType::Invalid);
                    case ERuleSubject::EntryPermit:
                        return std::make_pair(documents::DocType::EntryPermit, documents::PassportType::Invalid);
                    default:
                        return std::make_pair(documents::DocType::Invalid, documents::PassportType::Invalid);
                }
            }

            enum class ERuleTarget : u8
            {
                Invalid = static_cast<u8>(-1),
                Entrant = 0,
                Citizens,
                Foreigners,
                Workers,
                Diplomats,
                AsylumSeekers,
                Kolechians,
                FromImpor,
                FromUnitedFederation,
                FromAltanDistrict
            };

            struct RuleDescriptor
            {
                constexpr RuleDescriptor() = default;
                constexpr RuleDescriptor(ERuleAction action, ERuleSubject subject, ERuleTarget target)
                    : action{action}, subject{subject}, target{target}
                {}

                ERuleAction action = ERuleAction::Invalid;
                ERuleSubject subject = ERuleSubject::Invalid;
                ERuleTarget target = ERuleTarget::Invalid;
            };

            class Rule
            {
            public:
                constexpr Rule() = default;
                constexpr Rule(ERule rule, RuleDescriptor descriptor)
                    : m_rule{ rule }, m_descriptor{ descriptor }
                {}

            public:
                // void ApplyRule() const;
                ERule GetRule() const;
                std::string_view GetDescription() const;
                const RuleDescriptor& GetDescriptor() const;
            private:
                ERule m_rule = ERule::Invalid;
                RuleDescriptor m_descriptor{};
            };

            class RuleBook
            {
            public:
                RuleBook() = default;
                static constexpr size_t MaxRuleCount = 10;

                const core::FixedArray<Rule, MaxRuleCount>& GetRules() const
                {
                    return m_activeRules;
                }
                
            private:
                void RegisterRule(ERule rule);

            private:
                core::FixedArray<Rule, MaxRuleCount> m_activeRules{};

                friend std::optional<RuleBook> CreateRuleBook(const documents::Doc& document);
            };

            std::optional<RuleBook> CreateRuleBook(const documents::Doc& document);

        }  // namespace data
    }  // namespace analysis
}  // namespace paplease
