#pragma once
#include <optional>

#include "paplease/analysis/data/entrant_data.h"
#include "paplease/analysis/data/profile.h"
#include "paplease/analysis/data/rules.h"
#include "paplease/analysis/data/transcript.h"
#include "paplease/scannable/booth.h"
#include "paplease/types.h"

namespace paplease {
    namespace analysis {

        struct TrackedRule
        {
            enum class Status : u8 { Unmet, Complied, Broken };

            data::Rule rule;
            Status status;
        };

        class AnalysisContext
        {
        public:
            void SetEntrantCountry(data::ECountry country);
            void SetEntrantDistrict(data::EDistrict district);
            void SetEntrantCity(data::ECity city);
            void SetEntrantClassification(u8 classification);

            bool AddToProfile(const documents::Field& fieldData);

            bool HasRulebook() const noexcept;
            bool HasTranscript() const noexcept;

            const data::RuleBook& GetRulebook() const;
            const data::Transcript& GetTranscript() const;

            void SetRulebook(data::RuleBook&& rulebook);
            void SetTranscript(data::Transcript&& transcript);

            const core::FixedHashTable<data::ERule, TrackedRule, 10>& GetApplicableRules() const;

            void OnNewDate();
            void OnNewApplicant();

        public:
            void SetBoothFields(const scannable::BoothData& boothData);

            // Booth
            std::optional<documents::data::Date> currentDate;
            std::optional<documents::data::SIUnitValue> currentWeight;
            std::optional<documents::data::SIUnitValue> approximateHeight;
            int applicantNumber;

        private:
            void UpdateRules();

        private:
            // Store
            data::EntrantInfo m_entrant;
            data::Profile     m_profile;

            // Special document types
            std::optional<data::RuleBook> m_rulebook = std::nullopt;
            std::optional<data::Transcript> m_transcript = std::nullopt;
            
            // Rules
            core::FixedHashTable<data::ERule, TrackedRule, 10> m_applicableRules;
        };

    }
}
