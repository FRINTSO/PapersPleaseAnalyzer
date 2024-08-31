#pragma once
#include "base/game_state/mediators/analysis_mediator.h"

class State
{
public:
	State(AnalysisMediator* mediator)
		: m_mediator{ mediator }
	{}

protected:
	AnalysisMediator* m_mediator;
};