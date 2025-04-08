#pragma once
#include "base/analysis/analysis_context.h"
#include "base/analysis/components/applicant_state.h"
#include "base/analysis/components/document_state.h"
#include "base/analysis/components/game_state.h"
#include "base/analysis/document_analyzer.h"
#include "base/analysis/mediators/analysis_mediator.h"
#include "base/documents/doc_class.h"
#include "base/documents/data/date.h"
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
			void NotifyDocumentFound(const documents::Doc& document) override;

			const documents::Data& RequestData(const documents::DataType dataType) const override;
		private:
			AnalysisContext m_analysisContext;  // Context for all types of analysis

			// Temporary options - for development
			bool m_showGameView;
		};

	}  // namespace analysis
}  // namespace paplease
