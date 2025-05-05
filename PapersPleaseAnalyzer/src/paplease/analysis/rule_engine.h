#pragma once
#include "paplease/analysis/contexts/entrant_context.h"
#include "paplease/analysis/contexts/rule_context.h"
#include "paplease/analysis/data/rules.h"

namespace paplease {
    namespace analysis {

        class RuleEngine
        {
        public:
            RuleEngine(contexts::EntrantContext& entrant)
                : m_entrant(entrant) {}

            bool IsInitialized() { return m_rulebook != nullptr; }  // temporary - to turn of rules
            void Initialize(const data::RuleBook& rulebook);

            void UpdateApplicableRules();

        private:
            contexts::EntrantContext& m_entrant;
            contexts::RuleContext m_ruleCtx;

            const data::RuleBook* m_rulebook = nullptr;
        };

    }
}