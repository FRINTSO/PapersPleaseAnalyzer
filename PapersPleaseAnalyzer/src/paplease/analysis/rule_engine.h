#pragma once
#include "paplease/analysis/contexts/entrant_context.h"
#include "paplease/analysis/contexts/rule_context.h"
#include "paplease/analysis/data/rules.h"
#include "paplease/analysis/required_docs_tracker.h"

namespace paplease {
    namespace analysis {

        /*
        Either entrant_classification -> (what documents correspond to entrant_classification) -> require_documents
        or documents -> (is this document mentioned in rules) -> entrant_classification

        Where should this live?
        RuleEngine? RequiredDocsTracker? EntrantClassifier?

        1. It checks entrant_classification in both cases
        2. ClassifyEntrantBasedOnDocument does case 2
        3. UpdateApplicableRules does case 1

        What could I rename 2/3 to, to make the methods clearer?
        */

        class RuleEngine
        {
        public:
            RuleEngine(contexts::EntrantContext& entrant, RequiredDocsTracker& requiredDocsTracker)
                : m_entrant(entrant),
            m_requiredDocsTracker(requiredDocsTracker) {}

            bool IsInitialized() { return m_rulebook != nullptr; }
            void Initialize(const data::RuleBook& rulebook);


            void ResetEntrant()
            {
                m_ruleCtx.Reset();
            }
            void ResetDay() {
                m_ruleCtx.Reset();
                m_rulebook = nullptr;
            }

            void UpdateApplicableRules();
            void ClassifyEntrantBasedOnDocument(documents::AppearanceType appearanceType);

        private:
            void RequireDocumentsBasedOnEntrantClassification();

        private:
            contexts::EntrantContext& m_entrant;
            RequiredDocsTracker& m_requiredDocsTracker;

            contexts::RuleContext m_ruleCtx;
            const data::RuleBook* m_rulebook = nullptr;
        };

    }
}