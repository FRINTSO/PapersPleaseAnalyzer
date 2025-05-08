#pragma once
//#include "paplease/analysis/analysis_context.h"
//#include "paplease/analysis/contexts/game_context.h"
//#include "paplease/analysis/contexts/entrant_context.h"
//#include "paplease/analysis/rule_engine.h"
//#include "paplease/game_view.h"
//
//namespace paplease {
//	namespace analysis {
//
//		/*
//		State-machine
//		[Default case : Document analysis]
//		[Require document : waiting for that document to be found]
//
//
//		*/
//
//
//		class GameAnalyzer
//		{
//		public:
//			GameAnalyzer()
//				: m_gameContext(), m_entrantContext(),
//				m_documentPipeline(m_entrantContext, m_gameContext),
//				m_ruleEngine()
//			{}
//			void Scan(const GameView& gameView);
//
//		private:
//			void OnNewEntrant(int entrantNumber);
//			void OnNewDate(documents::data::Date date);
//
//		private:
//			// Contexts
//			contexts::EntrantContext m_entrantContext;
//			contexts::GameContext    m_gameContext;
//
//			// States
//			DocPipeline m_documentPipeline;
//
//			RuleEngine m_ruleEngine;
//		};
//
//	}  // namespace analysis
//}  // namespace paplease
