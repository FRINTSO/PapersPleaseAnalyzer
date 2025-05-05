#include "pch.h"
#include "rule_engine.h"

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
                {
                    contexts::TrackedRule trackedRule{ rule, contexts::TrackedRule::Status::Unmet };
#if DEBUG_LOG_APPLICABLE_RULES
                    LOG("Applicable rule: {}", trackedRule.rule.GetDescription());
#endif
                    m_ruleCtx.applicableRules.Set(eRule, std::move(trackedRule));
                }
            }
        }
    }
}
