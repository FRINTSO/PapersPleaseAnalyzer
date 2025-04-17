#include "pch.h"
#include "base/analysis/data/rules.h"

#include "base/utils/enum_range.h"
#include "base/utils/log.h"


namespace paplease {
	namespace analysis {
		namespace data {

			using namespace documents;

			static inline constexpr const Rule& FetchRule(ERule rule)
			{
				return {};
			}

			static inline constexpr const ERule GetERuleFromDescription(std::string_view ruleDescription)
			{
				return ERule::Invalid;
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
				// for (FieldCategory category = FieldCategory::Rule1; category <= FieldCategory::Rule10; ((int&)category)++)
				for (auto category : utils::enum_range(FieldCategory::Rule1, FieldCategory::Rule10))
				{
					auto rule = ruleData.GetField(category);
					auto eRule = GetERuleFromDescription(rule->get().ToString());
					auto fetchedRule = FetchRule(eRule);
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

		}  // namespace data
	}  // namespace analysis
}  // namespace paplease
