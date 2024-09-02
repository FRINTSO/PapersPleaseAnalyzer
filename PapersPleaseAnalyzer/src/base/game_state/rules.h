#pragma once
#include <array>
#include <optional>
#include <string>

#include "base/documents_v2/doc_class.h"

namespace paplease {
    namespace analysis {

        enum class ERule
        {
            Invalid = 0,

            RequireCurrentDocuments, // If this flag is set, mediator should know what functions to call

            RequirePassport, // Search for passport, if inspection can't locate it, and booth can't either, then ask where it is
            RequireArstotzkaPassport,	// Require that a passport is arstotzkan
            RequireIdentityCardFromCitizens,
            RequireEntryTicketFromForeigners,
            RequireWorkPassFromWorkers,
            RequireDiplomaticAuthorizationFromDiplomats,
            RequireIdentitySupplementFromForeigners,
            RequireGrantFromAsylumSeekers,
            RequirePolioVaccination,
            RequireAccessPermitFromForeigners,

            RequireSearchOfKolechians,
            ProhibitEntryFromImpor,

            ProhibitWeaponsAndContraband,
            ConfiscateArstotzkanPassportsFromAltanRegion,
            ProhibitEntryFromUnitedFederation,
            ConfiscateArstotzkanPassports,
        };

        class Rule
        {
        public:
            Rule() = default;

        public:
            void ApplyRule() const;
        private:
            std::string m_description;
            void* m_implication;
            void* m_inEffect;
            ERule m_rule;
        };

        class RuleBook
        {
        public:
            RuleBook() = default;

            friend std::optional<RuleBook> CreateRuleBook(const documents::v2::Doc& document);
        public:
            void ApplyRules() const;
        private:
            static constexpr size_t RuleCapacity = 10;
        private:
            std::array<Rule, RuleBook::RuleCapacity> m_activeRules;
            size_t m_ruleCount;
        };

        std::optional<RuleBook> CreateRuleBook(const documents::v2::Doc& document);

    }  // namespace analysis
}  // namespace paplease
