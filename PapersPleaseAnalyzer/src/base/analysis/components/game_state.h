#pragma once
#include "base/documents/data/date.h"
#include "base/documents/data/field_data.h"
#include "base/documents/doc_class.h"
#include "base/analysis/components/component.h"
#include "base/analysis/data/criminals.h"
#include "base/analysis/data/rules.h"
#include "base/analysis/data/transcript.h"
#include "base/analysis/scannable/scan_context.h"
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

				void Update(const scannable::ScanContext& scanContext);
			private: // State functions
				bool IsNewDate(documents::data::Date date);
				bool IsNewApplicant(int applicantNumber);
			public:
				const documents::data::Date& GetCurrentDate() const;

				bool HasCurrentRuleBook();
				bool HasCurrentCriminals();
			public: // Mediator functions

				void OnNewGameDate();
				void OnNewApplicant();

				void ReceiveRuleBookDocument(const documents::Doc& document);
				void ReceiveBulletinDocument(const documents::Doc& document);
				void ReceiveTranscriptDocument(const documents::Doc& document);
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
