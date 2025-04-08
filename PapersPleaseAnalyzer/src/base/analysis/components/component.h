#pragma once
#include "base/analysis/mediators/analysis_mediator.h"

namespace paplease {
	namespace analysis {
		namespace components {

			class Component
			{
			public:
				Component(mediators::AnalysisMediator* mediator)
					: m_mediator{ mediator }
				{}

			protected:
				mediators::AnalysisMediator* m_mediator;
			};

		}  // namespace components
	}  // namespace analysis
}  // namespace paplease
