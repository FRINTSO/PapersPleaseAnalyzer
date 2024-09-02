#pragma once
#include "base/documents_v2/doc_class.h"
#include "base/document_data/date.h"
#include "base/game_state/mediators/analysis_mediator.h"
#include "base/game_state/components/analyzer.h"
#include "base/game_state/components/booth.h"
#include "base/game_state/components/game_state.h"
#include "base/game_state/components/inspection.h"
#include "base/game_state/components/profiler_state.h"
#include "base/game_view.h"
#include "base/game_state/document_state.h"

// Context of current analysis
// Stores context of current analysis
// Stores and keeps track of documents relating to current applicant
// Also stores the game state

class GameAnalysisController : public AnalysisMediator
{
public:
	GameAnalysisController(bool showGameView = false);
	~GameAnalysisController();

	void Update(const GameView& gameView); // represents one round in the analysis chain
	void Notify(const Sender sender, const Event event) override;
	void NotifyDocumentFound(const Documents::V2::Doc& document) override;

	const Documents::Data::Date& RequestCurrentDate() override;
private:
	void UpdateBooth(const GameView& gameView);
	void UpdateInspection(const GameView& gameView);
	void UpdateGameState();
private:
	// Stateful
	GameState m_gameState; // Manages general game state, and game information, such as rules, countries, cities, criminals - stateful
	DocumentState m_documentState; // manages documents
	
	// Stateless
	BoothComponent m_booth; // Scans booth - Stateless
	InspectionComponent m_inspection; // Scans inspection Stateless

	ProfilerState m_profilerState; // Manages applicant profiling
	Analyzer m_analyzer; // Manages document analysis

	bool m_showGameView;
};