#include "pch.h"
#include "paplease/analysis/analysis_context.h"

namespace paplease {
    namespace analysis {

        void AnalysisContext::SetEntrantCountry(data::ECountry country)
        {
            m_entrant.SetNationaility(country);
            UpdateRules();
        }
        void AnalysisContext::SetEntrantDistrict(data::EDistrict district)
        {
            m_entrant.SetEntrantDistrict(district);
            UpdateRules();
        }
        void AnalysisContext::SetEntrantCity(data::ECity city)
        {
            m_entrant.SetEntrantCity(city);
            UpdateRules();
        }
        void AnalysisContext::SetEntrantClassification(u8 classification)
        {
            m_entrant.entrantClass |= classification;
            UpdateRules();
        }

        bool AnalysisContext::AddToProfile(const documents::Field& fieldData)
        {
            bool result = m_profile.RegisterData(fieldData);
            UpdateRules();
            return result;
        }

        bool AnalysisContext::HasRulebook() const noexcept
        {
            return m_rulebook.has_value();
        }

        bool AnalysisContext::HasTranscript() const noexcept
        {
            return m_transcript.has_value();
        }

        const data::RuleBook& AnalysisContext::GetRulebook() const
        {
            return m_rulebook.value();
        }

        const data::Transcript& AnalysisContext::GetTranscript() const
        {
            return m_transcript.value();
        }

        void AnalysisContext::SetRulebook(data::RuleBook&& rulebook)
        {
            m_rulebook = std::move(rulebook);
        }

        void AnalysisContext::SetTranscript(data::Transcript&& transcript)
        {
            m_transcript = std::move(transcript);
        }

        const core::FixedHashTable<data::ERule, TrackedRule, 10>& AnalysisContext::GetApplicableRules() const
        {
            return m_applicableRules;
        }

        void AnalysisContext::SetBoothFields(const scannable::BoothData& boothData)
        {
            currentDate = boothData.date;
            currentWeight = boothData.weight;
            approximateHeight = boothData.approximateHeight;
            applicantNumber = boothData.applicantNumber.value_or(0);
        }

        void AnalysisContext::UpdateRules()
        {
            if (!HasRulebook())
            {
                return;
            }

            // RenewApplicableRules
            // [Entrant type] -> [Required DocType]:
            // Entrant   -> Passport
            // Entrant   -> ArstotzkanPassport
            // Citizen   -> Id Card
            // Foreigner -> EntryTicket
            // Worker    -> WorkPass
            // Diplomat  -> DiplomaticAuth
            // Foreigner -> Id Supplement
            // Seeker    -> Grant
            // Entrant   -> PolioVaccine
            // Foreigner -> AccessPermit
            // Foreigner -> EntryPermit
            //
            for (const data::Rule rule : GetRulebook().GetRules())
            {
                const auto& [action, subject, target] = rule.GetDescriptor();
                const data::ERule eRule = rule.GetRule();
                
                if (m_entrant.entrantClass.IsTarget(target) && !m_applicableRules.Contains(eRule))
                {
                	TrackedRule trackedRule{ rule, TrackedRule::Status::Unmet };
#if DEBUG_LOG_APPLICABLE_RULES
                	LOG("Applicable rule: {}", trackedRule.rule.GetDescription());
#endif
                	m_applicableRules.Set(eRule, std::move(trackedRule));
                }
            }

            // RenewRequireDocumentRules
            
        }

        void AnalysisContext::OnNewDate()
        {
            currentDate = std::nullopt;
            currentWeight = std::nullopt;
            approximateHeight = std::nullopt;
            applicantNumber = 0;

            m_entrant = data::EntrantInfo();
            m_profile = data::Profile();

            m_rulebook = std::nullopt;
            m_transcript = std::nullopt;

            m_applicableRules.Clear();
        }

        void AnalysisContext::OnNewApplicant()
        {
            //currentDate = std::nullopt;
            currentWeight = std::nullopt;
            approximateHeight = std::nullopt;
            //applicantNumber = 0;

            m_entrant = data::EntrantInfo();
            m_profile = data::Profile();

            //m_rulebook = std::nullopt;
            m_transcript = std::nullopt;

            m_applicableRules.Clear();
        }

    }
}
