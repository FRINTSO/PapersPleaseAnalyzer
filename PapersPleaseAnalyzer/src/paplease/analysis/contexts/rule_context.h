#pragma once
#include "paplease/core/fixed.h"

namespace paplease {
    namespace analysis {
        namespace contexts {

            struct TrackedRule
            {
                enum class Status : u8 { Unmet, Complied, Broken };

                data::Rule rule;
                Status status;
            };

            struct RuleContext
            {
                core::FixedHashTable<data::ERule, TrackedRule, 10> applicableRules;

                void Reset()
                {
                    applicableRules.Clear();
                }
            };

        }  // namespace contexts
    }  // namespace analysis
}  // namespace paplease