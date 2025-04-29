#include "pch.h"
#include "paplease/analysis/analysis_context_v2.h"

namespace paplease {
    namespace analysis {

        void AnalysisContextV2::SetEntrantCountry(data::ECountry country)
        {
            m_entrant.SetNationaility(country);
            UpdateRules();
        }
        void AnalysisContextV2::SetEntrantDistrict(data::EDistrict district)
        {
            m_entrant.SetEntrantDistrict(district);
            UpdateRules();
        }
        void AnalysisContextV2::SetEntrantCity(data::ECity city)
        {
            m_entrant.SetEntrantCity(city);
            UpdateRules();
        }

        bool AnalysisContextV2::AddToProfile(const documents::Field& fieldData)
        {
            bool result = m_profile.RegisterData(fieldData);
            UpdateRules();
            return result;
        }

        bool AnalysisContextV2::HasRulebook() const noexcept
        {
            return m_rulebook.has_value();
        }

        bool AnalysisContextV2::HasTranscript() const noexcept
        {
            return m_transcript.has_value();
        }

        const data::RuleBook& AnalysisContextV2::GetRulebook() const
        {
            return m_rulebook.value();
        }

        const data::Transcript& AnalysisContextV2::GetTranscript() const
        {
            return m_transcript.value();
        }

        void AnalysisContextV2::SetRulebook(data::RuleBook&& rulebook)
        {
            m_rulebook = std::move(rulebook);
        }

        void AnalysisContextV2::SetTranscript(data::Transcript&& transcript)
        {
            m_transcript = std::move(transcript);
        }

        const core::FixedHashTable<data::ERule, TrackedRule, 10>& AnalysisContextV2::GetApplicableRules() const
        {
            return m_applicableRules;
        }

        void AnalysisContextV2::SetBoothFields(const scannable::BoothData& boothData)
        {
            currentDate = boothData.date;
            currentWeight = boothData.weight;
            approximateHeight = boothData.approximateHeight;
            applicantNumber = boothData.applicantNumber.value_or(0);
        }

        void AnalysisContextV2::UpdateRules()
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

        void AnalysisContextV2::OnNewDate()
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

        void AnalysisContextV2::OnNewApplicant()
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
