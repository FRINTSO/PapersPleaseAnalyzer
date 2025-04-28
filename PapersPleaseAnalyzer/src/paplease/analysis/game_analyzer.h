#pragma once
#include "paplease/analysis/data/entrant_data.h"
#include "paplease/analysis/data/rules.h"
#include "paplease/analysis/doc_tracker.h"
#include "paplease/game_view.h"
#include "paplease/scannable/scan_context.h"

namespace paplease {
	namespace analysis {

		class GameAnalyzer
		{
		public:
			void Scan(const GameView& gameView);

		private:
			bool TryGetRulebook();
			void RenewApplicableRules(const data::RuleBook& rulebook);
			void RenewRequireDocumentRules(const data::RuleBook& rulebook);
			void FigureOutEntrantInfo();

		private:
			// Per scan
			const GameView* m_currentGameView = nullptr;
			scannable::ScanContext m_currentScanContext;

			// States
			DocTrackerV2 m_documentTracker;
			std::optional<data::RuleBook> m_rulebook = std::nullopt;

			// Data
			data::EntrantInfo m_entrant;

			struct TrackedRule
			{
				enum class Status : u8 { Unmet, Complied, Broken };

				data::Rule rule;
				Status status;
			};
			
			core::FixedHashTable<data::ERule, TrackedRule, 10> m_applicableRules;
			
			
		};

	}  // namespace analysis
}  // namespace paplease
