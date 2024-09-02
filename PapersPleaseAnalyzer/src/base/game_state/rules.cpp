#include "pch.h"
#include "base/game_state/rules.h"

#include "base/utils/log.h"


namespace paplease {
	namespace analysis {

		using namespace documents::v2;

		static inline constexpr const Rule& FetchRule(ERule rule)
		{
			return {};
		}

		std::optional<RuleBook> CreateRuleBook(const Doc& document)
		{
			auto ruleData = document.GetDocumentData();

			if (!document.IsValid())
			{
				LOG_ERR("Tried to create rule book from invalid document");
				return std::nullopt;
			}

			// for (int i = (int)DataFieldCategory::Rule1; i <= (int)DataFieldCategory::Rule10; i++)
			for (DataFieldCategory category = DataFieldCategory::Rule1; category <= DataFieldCategory::Rule10; ((int&)category)++)
			{
				ruleData.Get(category);
			}

			return RuleBook{};
		}

		void Rule::ApplyRule() const
		{

		}

		void RuleBook::ApplyRules() const
		{
			for (const auto& rule : m_activeRules)
			{
				rule.ApplyRule();
			}
		}


	}  // namespace analysis
}  // namespace paplease
