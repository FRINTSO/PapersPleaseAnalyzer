#pragma once
#include "base/documents_v2/doc_class.h"
#include "base/document_data/date.h"
#include "base/game_state/mediators/analysis_mediator.h"
#include "base/game_state/components/analyzer.h"
#include "base/game_state/components/booth.h"
#include "base/game_state/components/document_state.h"
#include "base/game_state/components/game_state.h"
#include "base/game_state/components/inspection.h"
#include "base/game_state/components/profiler.h"
#include "base/game_view.h"

// Context of current analysis
// Stores context of current analysis
// Stores and keeps track of documents relating to current applicant
// Also stores the game state

namespace paplease {
	namespace analysis {

		class GameAnalysisController : public mediators::AnalysisMediator
		{
		public:
			GameAnalysisController(bool showGameView = false);
			~GameAnalysisController();

			void Update(const GameView& gameView); // represents one round in the analysis chain
			void Notify(const mediators::Sender sender, const mediators::Event event) override;
			void NotifyDocumentFound(const documents::v2::Doc& document) override;

			const documents::v2::Data& RequestData(const documents::v2::DataType dataType) const override;
			const documents::data::Date& RequestCurrentDate() const override;
		private:
			bool UpdateBooth(const GameView& gameView);
			void UpdateInspection(const GameView& gameView);
			void UpdateGameState();
			void DocumentAnalysis();
		private:
			// Stateful
			components::GameState m_gameState; // Manages general game state, and game information, such as rules, countries, cities, criminals - stateful
			components::DocumentState m_documentState; // manages documents

			// Stateless
			components::BoothComponent m_booth; // Scans booth - Stateless
			components::InspectionComponent m_inspection; // Scans inspection Stateless

			components::ApplicantProfiler m_profiler; // Manages applicant profiling
			components::DocumentAnalyzer m_analyzer; // Manages document analysis

			// Temporary options - for development
			bool m_showGameView;
		};

	}  // namespace analysis
}  // namespace paplease
