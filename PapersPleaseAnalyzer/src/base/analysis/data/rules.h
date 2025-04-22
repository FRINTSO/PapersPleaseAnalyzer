#pragma once
#include <array>
#include <optional>
#include <string>

#include "base/documents/doc_class.h"
#include "base/utils/fixed_array.h"

namespace paplease {
    namespace analysis {
        namespace data {

            enum class ERule : unsigned int
            {
                Invalid = -1,

                // === REQUIREMENTS ===
                RequireCurrentDocumentsFromEntrant, // If this flag is set, mediator should know what functions to call
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

            enum class ERuleAction : unsigned char
            {
                Require,    // Need information
                Prohibit,   // Limit
                Confiscate  // Limit
            };

            enum class ERuleSubject : unsigned char
            {
                CurrentDocuments,
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

            enum class ERuleTarget : unsigned char
            {
                Entrant,
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
                consteval RuleDescriptor( ERuleAction action, ERuleSubject subject, ERuleTarget target)
                    : action{action}, subject{subject}, target{target}
                {}

                ERuleAction action;
                ERuleSubject subject;
                ERuleTarget target;
            };

            class Rule
            {
            public:
                consteval Rule(ERule rule, RuleDescriptor descriptor)
                    : m_rule{ rule }, m_descriptor{ descriptor }
                {}

            public:
                // void ApplyRule() const;
                ERule GetRule() const;
                std::string_view GetDescription() const;
                const RuleDescriptor& GetDescriptor() const;
            private:
                ERule m_rule;
                RuleDescriptor m_descriptor;
            };

            class RuleBook
            {
            public:
                RuleBook() = default;
                static constexpr size_t MaxRuleCount = 10;

                const utils::FixedArray<const Rule*, MaxRuleCount>& GetRules() const
                {
                    return m_activeRules;
                }
                
            private:
                void RegisterRule(ERule rule);

            private:
                utils::FixedArray<const Rule*, MaxRuleCount> m_activeRules{nullptr};

                friend std::optional<RuleBook> CreateRuleBook(const documents::Doc& document);
            };

            std::optional<RuleBook> CreateRuleBook(const documents::Doc& document);

        }  // namespace data
    }  // namespace analysis
}  // namespace paplease
