#pragma once
#include "base/documents_v2/doc_class.h"
#include "base/document_data/date.h"

namespace paplease {
	namespace analysis {
		namespace mediators {

			enum class Sender
			{
				GameState,
				BoothState,
				InspectionState
			};

			enum class Event
			{
				NewGameDate,
				NewApplicant,
				FindRuleBook,
				RuleBookFound,
				FindBulletin,
				BulletinFound,
				FindTranscript,
				TranscriptFound,
			};

			class AnalysisMediator
			{
			public:
				~AnalysisMediator() {}
				virtual void Notify(const Sender sender, const Event event) = 0;
				virtual void NotifyDocumentFound(const documents::v2::Doc& document) = 0;

				virtual const documents::data::Date& RequestCurrentDate() = 0;
			};

		}  // namespace mediators
	}  // namespace analysis
}  // namespace paplease
