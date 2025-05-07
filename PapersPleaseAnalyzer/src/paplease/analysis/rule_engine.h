#pragma once
#include "paplease/analysis/contexts/entrant_context.h"
#include "paplease/analysis/contexts/rule_context.h"
#include "paplease/analysis/data/rules.h"
#include "paplease/analysis/required_docs_tracker.h"

namespace paplease {
    namespace analysis {

        class RuleEngine
        {
        public:
            RuleEngine(contexts::EntrantContext& entrant, RequiredDocsTracker& requiredDocsTracker)
                : m_entrant(entrant),
            m_requiredDocsTracker(requiredDocsTracker) {}

            bool IsInitialized() { return m_rulebook != nullptr; }  // temporary - to turn of rules
            void Initialize(const data::RuleBook& rulebook);
            void ResetEntrant()
            {
                m_ruleCtx.Reset();
            }
            void Reset() {
                m_ruleCtx.Reset();
                m_rulebook = nullptr;
            }


            void UpdateApplicableRules();

            void ClassifyEntrantBasedOnDocument(documents::AppearanceType appearanceType);

        private:
            contexts::EntrantContext& m_entrant;
            RequiredDocsTracker& m_requiredDocsTracker;

            contexts::RuleContext m_ruleCtx;

            const data::RuleBook* m_rulebook = nullptr;
        };

    }
}