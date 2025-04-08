#pragma once
#include "base/documents/data/date.h"
#include "base/documents/doc_class.h"

// Analyzer, two states
// Analyze can compare a document against itself and available gameData
// It can also compare it against the profile

namespace paplease {
	namespace analysis {

		struct DocAnalysis
		{
			bool hasDiscrepancy;
		};

		DocAnalysis AnalyzeDocumentValidity(const documents::Doc& document, const documents::data::Date& currentDate);

	}  // namespace analysis
}  // namespace paplease
