#pragma once
#include "paplease/analysis/data/entrant_data.h"
#include "paplease/analysis/data/profile.h"
#include "paplease/analysis/data/transcript.h"
#include "paplease/core/fixed.h"
#include "paplease/documents/data/field_data.h"
#include "paplease/documents/doc_type.h"
#include "paplease/documents/doc_data.h"

#include <optional>

namespace paplease {
    namespace analysis {
        namespace contexts {

            class DocMemoryStore
            {
            public:
                DocMemoryStore()
                {

                }
                core::FixedTable<documents::DocType, documents::DocData> data;
            };

            class EntrantContext // Lives only for one applicant
            {
            public:
                DocMemoryStore& GetMemoryStore() { return m_memoryStore; }
                data::EntrantInfo& GetEntrantInfo() { return m_entrant; }
                data::Profile& GetProfile() { return m_profile; }
                void SetWeight(documents::data::SIUnitValue weight) { m_weight = weight; }
                std::optional<documents::data::SIUnitValue> GetWeight() const { return m_weight; }
                void SetHeight(documents::data::SIUnitValue height) { m_approximateHeight = height; }
                std::optional<documents::data::SIUnitValue> GetApproximateHeight() const { return m_approximateHeight; }
                void SetAnalysisCompleted() noexcept { m_currentAnalysisCompleted = true; }
                bool AnalysisCompleted() const noexcept { return m_currentAnalysisCompleted; }
                void Reset()
                {
                    m_entrant = data::EntrantInfo{};
                    m_profile = data::Profile{};
                    m_memoryStore = DocMemoryStore{};
                    m_weight = std::nullopt;
                    m_approximateHeight = std::nullopt;
                    m_transcript = std::nullopt;

                    m_currentAnalysisCompleted = false;
                }
            private:
                data::EntrantInfo m_entrant;
                data::Profile     m_profile;
                DocMemoryStore    m_memoryStore;
                std::optional<documents::data::SIUnitValue> m_weight;  // needs to be set
                std::optional<documents::data::SIUnitValue> m_approximateHeight;  // needs to be set
                std::optional<data::Transcript> m_transcript;

                bool m_currentAnalysisCompleted = false;
            };

        }  // namespace contexts
    }  // namespace analysis
}  // namespace paplease