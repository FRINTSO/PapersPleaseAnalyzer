#pragma once
#include "base/documents_v2/doc_class.h"
#include "base/game_state/rules.h"
#include "base/game_state/components/component.h"

// Analyzer, two states
// Analyze can compare a document against itself and available gameData
// It can also compare it against the profile

namespace paplease {
	namespace analysis {
		namespace components {

class Analysis {
private:
	bool hasDiscrepancy;
};

class Analyzer : public Component
{
public:
	using Component::Component;

public: // Mediator functions
	Analysis AnalyzeDocumentValidity(const documents::v2::Doc& document) const;
private:
	bool DocumentHasCurrentDate(const documents::v2::Doc& document) const;
};

		}  // namespace components
	}  // namespace analysis
}  // namespace paplease
