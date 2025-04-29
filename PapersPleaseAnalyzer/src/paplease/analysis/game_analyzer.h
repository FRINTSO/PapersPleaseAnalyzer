#pragma once
#include "paplease/analysis/analysis_context_v2.h"
#include "paplease/analysis/data/rules.h"
#include "paplease/analysis/doc_pipeline.h"
#include "paplease/game_view.h"
#include "paplease/scannable/scan_context.h"

namespace paplease {
	namespace analysis {

		class GameAnalyzer
		{
		public:
			constexpr GameAnalyzer()
				: m_analysisContext(), m_documentPipeline(m_analysisContext) {}
			void Scan(const GameView& gameView);

		private:
			void UpdateAnalysisContext();

			bool IsNewDate(const documents::data::Date& date) const noexcept;
			bool IsNewApplicant(int applicantNumber) const noexcept;

			void OnNewDate();
			void OnNewApplicant();

		private:
			// Per scan
			const GameView* m_currentGameView = nullptr;
			scannable::ScanContext m_currentScanContext;

			// States
			DocPipeline m_documentPipeline;

			// Data
			AnalysisContextV2 m_analysisContext;
		};

	}  // namespace analysis
}  // namespace paplease
