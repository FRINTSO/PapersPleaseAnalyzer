#pragma once
#include "base/documents_v2/doc_class.h"
#include "base/document_data/date.h"
#include "base/document_data/field_data.h"
#include "base/game_state/data/criminals.h"
#include "base/game_state/data/rules.h"
#include "base/game_state/data/transcript.h"
#include "base/game_state/components/component.h"
#include "base/game_view.h"

// Tracks the games state
// Including booth data
// Includes game rules for chosen day
//

namespace paplease {
	namespace analysis {
		namespace components {

			class BoothDoc;


			class GameState final : public Component
			{
			public:
				using Component::Component;

				// void Update(const GameView& gameView);

			public: // State functions
				bool IsNewDate(documents::data::Date date);
				bool IsNewApplicant(int applicantNumber);

				const documents::data::Date& GetCurrentDate() const;

				bool HasCurrentRuleBook();
				bool HasCurrentCriminals();
			public: // Mediator functions

				void OnNewGameDate();
				void OnNewApplicant();

				void ReceiveRuleBookDocument(const documents::v2::Doc& document);
				void ReceiveBulletinDocument(const documents::v2::Doc& document);
				void ReceiveTranscriptDocument(const documents::v2::Doc& document);
			private:
				// State
				documents::data::Date m_currentDate;
				int m_applicantNumber;

				bool m_hasCurrentRules;
				bool m_hasCurrentCriminals;

				data::RuleBook m_ruleBook;
				data::CriminalData m_criminalData;
				data::Transcript m_transcript;
			};

		}  // namespace components
	}  // namespace analysis
}  // namespace paplease
