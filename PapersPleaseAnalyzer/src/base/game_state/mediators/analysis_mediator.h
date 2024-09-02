#pragma once
#include "base/documents_v2/doc_class.h"
#include "base/document_data/date.h"

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
	virtual void NotifyDocumentFound(const Documents::V2::Doc& document) = 0;

	virtual const Documents::Data::Date& RequestCurrentDate() = 0;
};