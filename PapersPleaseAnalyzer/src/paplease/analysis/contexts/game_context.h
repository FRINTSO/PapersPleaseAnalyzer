#pragma once

#include "paplease/analysis/data/criminals.h"
#include "paplease/analysis/data/rules.h"
#include "paplease/documents/data/date.h"

#include <optional>

namespace paplease {
    namespace analysis {
        namespace contexts {

            class GameContext // Lives for the entire day
            {
            public:
                GameContext() = default;

                void SetRuleBook(data::RuleBook&& rulebook)
                {
                    m_rulebook = rulebook;
                }
                const data::RuleBook& GetRuleBook() const { return m_rulebook.value(); }
                void SetCurrentDate(documents::data::Date date)
                {
                    m_currentDate = date;
                }
                void SetEntrantNumber(int number)
                {
                    m_entrantNumber = number;
                }
                std::optional<documents::data::Date> GetCurrentDate() const { return m_currentDate; }
                int GetEntrantNumber() const { return m_entrantNumber; }
                void Reset()
                {
                    m_rulebook = std::nullopt;
                    m_criminals = std::nullopt;
                    m_currentDate = std::nullopt;
                    m_entrantNumber = 0;
                }
            private:
                std::optional<data::RuleBook> m_rulebook;
                std::optional<data::CriminalData> m_criminals;
                std::optional<documents::data::Date> m_currentDate;
                int m_entrantNumber = 0;
            };

        }  // namespace contexts
    }  // namespace analysis
}  // namespace paplease