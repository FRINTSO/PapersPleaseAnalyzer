#pragma once
#include "paplease/analysis/analysis_context_v2.h"
#include "paplease/analysis/doc_store.h"
#include "paplease/documents/doc_type.h"

namespace paplease {
    namespace analysis {

        class DocAnalyzer
        {
        public:
            constexpr DocAnalyzer(DocStore& store, AnalysisContextV2& analysisContext)
                : m_store(store), m_context(analysisContext) {}

            bool AnalyzeDocumentData(documents::DocType documentType);

        private:
            bool AnalyzeEntrantDocument(documents::DocType documentType);
            bool AnalyzeRuleBook();
            bool AnalyzeTranscript();

            class DocValidator
            {
			public:
				constexpr DocValidator(
					AnalysisContextV2& analysisContext,
					const documents::Doc& document,
					const documents::DocData& documentData
				)
					: m_context(analysisContext),
					m_document(document),
					m_documentData(documentData)
				{}

				bool Validate();

			private:
				// Against static information, ex. Issuing city, or current date
				bool ValidateExpirationDate() const;
				bool ValidateIssuingCity() const;
				bool ValidateDistrict() const;
				bool ValidateForgedOrMissingSeal() const;
				bool ValidateEntryTicketValidOnDate() const;
				bool ValidateIssuingCountry() const;
				bool ValidateAccessToAristotzka() const;
				bool ValidateMissingVaccine() const;
				bool ValidateVaccineExpirationDate() const;

				// Against booth
				bool ValidateWeight() const;

				// Against images - not supported yet
				bool ValidateSex() const;
				bool ValidatePhoto() const;
				bool ValidateHeight() const;
				bool ValidatePhysicalAppearance() const;
				bool ValidateThumbprint() const;
				bool ValidateFingerprints() const;

				// Against other applicant documents
				bool ValidateName() const;
				bool ValidateDateOfBirth() const;
				bool ValidatePassportNumber() const;
				bool ValidateWorkEndDate() const;

			private:
				AnalysisContextV2& m_context;
				const documents::Doc& m_document;  // The document being validated
				const documents::DocData& m_documentData;
            };

        private:
            DocStore& m_store;
            AnalysisContextV2& m_context;
        };

    }  // namespace analysis
}  // namespace paplease