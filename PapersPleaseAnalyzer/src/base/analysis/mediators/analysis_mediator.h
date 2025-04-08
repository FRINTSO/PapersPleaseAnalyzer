#pragma once
#include "base/documents/doc_class.h"
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
				virtual void NotifyDocumentFound(const documents::Doc& document) = 0;

				virtual const documents::Data& RequestData(const documents::DataType dataType) const = 0;
			};

		}  // namespace mediators
	}  // namespace analysis
}  // namespace paplease
