#include "pch.h"
#include "paplease/analysis/game_analyzer.h"

#include <utility>

#include "paplease/core/fixed.h"

namespace paplease {
	namespace analysis {

		void GameAnalyzer::Scan(const GameView& gameView)
		{
			m_currentGameView = &gameView;
			m_currentScanContext = scannable::Scan(gameView);

			m_documentTracker.RefreshTracking(m_currentScanContext);

			if (TryGetRulebook())
			{
				RenewApplicableRules(*m_rulebook);
				RenewRequireDocumentRules(*m_rulebook);

				FigureOutEntrantInfo();
			}
		}

		bool GameAnalyzer::TryGetRulebook()
		{
			if (m_rulebook != std::nullopt)
			{
				return true;
			}

			if (m_documentTracker.IsVisible(documents::DocType::RuleBook, ViewArea::InspectionView))
			{
				documents::Doc rulebookDocument;
				m_documentTracker.ExtractDoc(*m_currentGameView, documents::DocType::RuleBook, rulebookDocument);
				auto rulebookOpt = data::CreateRuleBook(rulebookDocument);
				if (rulebookOpt)
				{
					m_rulebook = rulebookOpt.value();
					LOG("Rulebook has been scanned!");

#if DEBUG_LOG_RULES
					LOG_RAW("Rules:");
					for (const auto& rule : m_rulebook->GetRules())
					{
						LOG("{}", rule.GetDescription());
					}
#endif
					return true;
				}
			}
			return false;
		}

		void GameAnalyzer::RenewApplicableRules(const data::RuleBook& rulebook)
		{
			// [Entrant type] -> [Required DocType]:
			// Entrant   -> Passport
			// Entrant   -> ArstotzkanPassport
			// Citizen   -> Id Card
			// Foreigner -> EntryTicket
			// Worker    -> WorkPass
			// Diplomat  -> DiplomaticAuth
			// Foreigner -> Id Supplement
			// Seeker    -> Grant
			// Entrant   -> PolioVaccine
			// Foreigner -> AccessPermit
			// Foreigner -> EntryPermit
			//

			for (const auto rule : rulebook.GetRules())
			{
				const auto& [action, subject, target] = rule.GetDescriptor();
				const data::ERule eRule = rule.GetRule();

				if (m_entrant.entrantClass.IsTarget(target) && !m_applicableRules.Contains(eRule))
				{
					TrackedRule trackedRule{ rule, TrackedRule::Status::Unmet };
#if DEBUG_LOG_APPLICABLE_RULES
					LOG("Applicable rule: {}", trackedRule.rule.GetDescription());
#endif
					m_applicableRules.Set(eRule, std::move(trackedRule));
				}
			}
		}

		void GameAnalyzer::RenewRequireDocumentRules(const data::RuleBook& rulebook)
		{
			for (const auto trackedRule : m_applicableRules)
			{
				const auto rule = trackedRule->rule;
				switch (rule.GetRule())
				{
					case data::ERule::RequirePassportFromEntrant:
					{
						documents::DocView docView;
						if (m_documentTracker.m_documents.Get(documents::DocType::Passport, docView))
						{
							// Check if is citizen
							if (docView.appearanceType == documents::AppearanceType::Passport_Arstotzka)
							{
								LOG("Is citizen");
								m_entrant.entrantClass = m_entrant.entrantClass | data::EntrantClass::Citizen;
							}
							else
							{
								LOG("Is foreigner");
								m_entrant.entrantClass = m_entrant.entrantClass | data::EntrantClass::Foreigner;
							}
						}
					}
					case data::ERule::RequireArstotzkanPassportFromEntrant:
					case data::ERule::RequireIdentityCardFromCitizens:
					case data::ERule::RequireEntryTicketFromForeigners:
					case data::ERule::RequireWorkPassFromWorkers:
					case data::ERule::RequireDiplomaticAuthorizationFromDiplomats:
					case data::ERule::RequireIdentitySupplementFromForeigners:
					case data::ERule::RequireGrantFromAsylumSeekers:
					case data::ERule::RequirePolioVaccinationFromEntrant:
					case data::ERule::RequireAccessPermitFromForeigners:
					case data::ERule::RequireEntryPermitFromForeigners:
					case data::ERule::RequireSearchOfKolechians:
					{
						const auto docType = data::ERuleSubjectToDocType(rule.GetDescriptor().subject);
						m_documentTracker.AddRequiredDocument(docType);
						continue;
					}
					default:
						continue;
				}
			}
		}

		void GameAnalyzer::FigureOutEntrantInfo()
		{

		}

	}  // namespace analysis
}  // namespace paplease
