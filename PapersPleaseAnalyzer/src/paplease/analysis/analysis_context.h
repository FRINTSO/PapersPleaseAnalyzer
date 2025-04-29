#pragma once
#include <optional>

#include "paplease/analysis/data/criminals.h"
#include "paplease/analysis/data/rules.h"
#include "paplease/analysis/data/transcript.h"
#include "paplease/analysis/data/entrant_data.h"
#include "paplease/analysis/doc_tracker.h"
#include "paplease/scannable/scan_context.h"
#include "paplease/documents/data/date.h"
#include "paplease/documents/doc_class.h"
#include "paplease/documents/doc_type.h"

namespace paplease {
	namespace analysis {
		
		class DocRegistry
		{
		public:
			using DocRef = std::reference_wrapper<const documents::Doc>;
		
			bool AcquireIfNewDocument(documents::Doc&& document);
			std::optional<DocRef> GetDocumentByType(const documents::DocType documentType) const;

		private:
			void AcquireDocument(documents::Doc&& document);
			bool IsNewDocument(const documents::Doc& document) const;
			bool HasDocumentOfType(documents::DocType documentType) const;

		public:
			void OnNewDate();
			void OnNewApplicant();

		private:
			static constexpr size_t DocumentCapacity = 10;
		private:
			std::array<documents::Doc, DocumentCapacity> m_documents;
			size_t m_documentCount;
		};
		
		class Profile
		{
		public:
			static constexpr int MatchingData = 1;
			static constexpr int NoData = 0;
			static constexpr int MismatchingData = -1;

			void OnNewDate();
			void OnNewApplicant();

			bool RegisterData(const documents::Field& fieldData);
		private:
			void Clear();
			int CompareData(const documents::Field& fieldData) const;

			static constexpr size_t FieldsCapacity = 20;
			static constexpr std::array<documents::FieldCategory, FieldsCapacity> ProfileFields();
			static constexpr int GetFieldIndexByCategoryType(documents::FieldCategory category);
		private:
			std::array<std::optional<documents::Field>, Profile::FieldsCapacity> m_fields;
			size_t m_fieldCount;
		};

		class AnalysisContext  // Object containing everything necessary for any analysis to take place
		{
		public:
			void Update(scannable::ScanContext& scanContext);

		private:
			bool SetIfNewDate(documents::data::Date date);
			bool SetIfNewApplicant(int applicantNumber);

			void OnNewDate(documents::data::Date date);
			void OnNewApplicant(int applicantNumber);

		private: // DocRegistry:
			void SaveScannedDocuments(scannable::InspectionData& inspectionData, const GameView& gameView);
			void HandleDocumentByType(documents::DocType documentType);

			void StoreBulletin(const documents::Doc& document);
			void StoreRuleBook(const documents::Doc& document);
			void StoreTranscript(const documents::Doc& document);

			void ReiterateRulebook() const;

		private:  // Profiler:
			void AddToProfile(const documents::Doc& document);
			void ValidateDocument(const documents::Doc& document);

			friend class DocumentValidator;
		private:
			// Plain data
			std::optional<documents::data::Date> m_currentDate;
			std::optional<documents::data::SIUnitValue> m_currentWeight;
			std::optional<documents::data::SIUnitValue> m_approximateHeight;
			int m_applicantNumber;

			std::optional<data::RuleBook> m_ruleBook;
			std::optional<data::CriminalData> m_criminalData;
			std::optional<data::Transcript> m_transcript;

			// More complex
			DocTracker m_docTracker;
			DocRegistry m_docRegistry;  // stores all documents currently in effect

			// Profiler:
			Profile m_profile;
			data::EntrantInfo m_entrantInfo;
		};

		class DocumentValidator
		{
		public:
			DocumentValidator(const documents::Doc& document, AnalysisContext& analysisContext);

			bool Validate();

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

			// Against rulebook, transcript, bulletin
			//bool ValidateAgainstRulebook() const;
			//bool ValidateAgainstTranscript() const;

			// Against other applicant documents
			bool ValidateName() const;
			bool ValidateDateOfBirth() const;
			bool ValidatePassportNumber() const;
			bool ValidateWorkEndDate() const;

		private:
			AnalysisContext& m_analysisContext;
			const documents::Doc& m_document;  // The document being validated
			documents::DocData m_documentData;
		};

	}  // namespace analysis
}  // namespace paplease

#include "paplease/analysis/analysis_context.inl.h"
