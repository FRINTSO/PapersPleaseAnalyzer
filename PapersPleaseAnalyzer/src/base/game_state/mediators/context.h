#pragma once
#include "base/documents_v2/doc_class.h"
#include "base/game_state/mediators/analysis_mediator.h"
#include "base/game_state/states/booth_state.h"
#include "base/game_state/states/game_state.h"
#include "base/game_state/states/inspection_state.h"
#include "base/game_state/states/profiler_state.h"
#include "base/game_view.h"

// Context of current analysis
// Stores context of current analysis
// Stores and keeps track of documents relating to current applicant
// Also stores the game state

class AnalysisContext : public AnalysisMediator
{
public:
	AnalysisContext(bool showGameView = false);
	~AnalysisContext();

	void Update(const GameView& gameView); // represents one round in the analysis chain
	void Notify(const Sender sender, const Event event) override;
	void NotifyDocumentFound(const Documents::V2::Doc& document) override;

private:
	GameState m_gameState;
	BoothState m_boothState;
	InspectionState m_inspectionState;
	ProfilerState m_profilerState;

	bool m_showGameView;
};