#pragma once
#include "paplease/analysis/contexts/frame_context.h"
#include "paplease/analysis/contexts/entrant_context.h"
#include "paplease/analysis/contexts/game_context.h"
#include "paplease/analysis/doc_store.h"
#include "paplease/documents/doc_type.h"

namespace paplease {
    namespace analysis {

        class DocAnalyzer
        {
        public:
            constexpr DocAnalyzer(contexts::FrameContext& frame, contexts::EntrantContext& entrant, contexts::GameContext& game)
                : m_frame(frame), m_entrant(entrant), m_game(game) {}

			bool AnalyzeDocument(documents::DocType documentType);

   //     private:
   //         bool AnalyzeEntrantDocument(documents::DocType documentType);
   //         bool AnalyzeRuleBook();
   //         bool AnalyzeTranscript();

            class DocValidator
            {
			//public:
			//	constexpr DocValidator(
			//		const documents::Doc& document,
			//		const documents::DocData& documentData
			//	)
			//		:
			//		m_document(document),
			//		m_documentData(documentData)
			//	{}

			//	bool Validate();

			//private:
			//	// Against static information, ex. Issuing city, or current date
			//	bool ValidateExpirationDate() const;
			//	bool ValidateIssuingCity() const;
			//	bool ValidateDistrict() const;
			//	bool ValidateForgedOrMissingSeal() const;
			//	bool ValidateEntryTicketValidOnDate() const;
			//	bool ValidateIssuingCountry() const;
			//	bool ValidateAccessToAristotzka() const;
			//	bool ValidateMissingVaccine() const;
			//	bool ValidateVaccineExpirationDate() const;
			//	bool ValidatePurpose() const;

			//	// Against booth
			//	bool ValidateWeight() const;

			//	// Against images - not supported yet
			//	bool ValidateSex() const;
			//	bool ValidatePhoto() const;
			//	bool ValidateHeight() const;
			//	bool ValidatePhysicalAppearance() const;
			//	bool ValidateThumbprint() const;
			//	bool ValidateFingerprints() const;

			//	// Against other applicant documents
			//	bool ValidateName() const;
			//	bool ValidateDateOfBirth() const;
			//	bool ValidatePassportNumber() const;
			//	bool ValidateWorkEndDate() const;

			private:
				const documents::Doc& m_document;  // The document being validated
				const documents::DocData& m_documentData;
            };

        private:
			contexts::FrameContext&   m_frame;
			contexts::EntrantContext& m_entrant;
			contexts::GameContext&    m_game;
        };

    }  // namespace analysis
}  // namespace paplease