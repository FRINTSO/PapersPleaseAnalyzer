#include "pch.h"
#include "paplease/analysis/rule_engine.h"

#include "paplease/analysis/data/location_bank.h"
#include "paplease/documents/doc_lookup.h"

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
                const data::ERule eRule = rule.GetRule();
                if (m_ruleCtx.applicableRules.Contains(eRule))
                {  // rule is already applicable
                    continue;
                }

                if (rule.AppliesTo(m_entrant.GetEntrantInfo()))
                {  // the rule applies to the current understanding of whom the entrant is
#if DEBUG_LOG_APPLICABLE_RULES
                    LOG("Applicable rule: {}", rule.GetDescription());
#endif
                    m_ruleCtx.applicableRules.Set(rule.GetRule(), { rule, contexts::TrackedRule::Status::Unmet });

                    if (rule.GetAction() == data::ERuleAction::RequireDocument)
                    {
                        documents::DocLookup lookupType = data::ERuleSubjectToDocType(rule.GetSubject());
                        m_requiredDocsTracker.AddRequiredDocument(lookupType);
                    }
                }
            }
        }

        void RuleEngine::ClassifyEntrantBasedOnDocument(documents::AppearanceType appearanceType)
        {
            if (m_rulebook == nullptr)
            {
                LOG_ERR("Tried to call 'ClassifyEntrantBasedOnDocument' when m_rulebook is null");
                return;
            }

            documents::DocLookup lookupType{ appearanceType };
            for (const data::Rule rule : m_rulebook->GetRules())
            {
                if (rule.GetAction() == data::ERuleAction::RequireDocument)
                {
                    const auto ruleLookupType = data::ERuleSubjectToDocType(rule.GetSubject());
                    if (lookupType.AppliesTo(ruleLookupType))
                    {
                        m_entrant.GetEntrantInfo().SetEntrantClassification(rule.GetTargetEntrantClass());
                        break;
                    }
                }
            }

            this->UpdateApplicableRules();
        }

        void RuleEngine::RequireDocumentsBasedOnEntrantClassification()
        {
            for (const data::Rule rule : m_rulebook->GetRules())
            {
                if (m_ruleCtx.applicableRules.Contains(rule.GetRule()))
                {
                    continue;
                }

                if (rule.AppliesTo(m_entrant.GetEntrantInfo()) && rule.GetAction() == data::ERuleAction::RequireDocument)
                {
                    documents::DocLookup lookupType = data::ERuleSubjectToDocType(rule.GetSubject());
                    m_requiredDocsTracker.AddRequiredDocument(lookupType);

                    m_ruleCtx.applicableRules.Set(rule.GetRule(), { rule, contexts::TrackedRule::Status::Unmet });
                }
            }
        }

    }  // namespace analysis
}  // namespace paplease
