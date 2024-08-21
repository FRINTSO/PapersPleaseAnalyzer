#pragma once
#include "base/game_view.h"


struct Analysis {
	bool hasDiscrepancy;
};


Analysis Analyze(const GameView& gameView);