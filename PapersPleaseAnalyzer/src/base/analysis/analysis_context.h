#pragma once
#include "base/analysis/data/criminals.h"
#include "base/analysis/data/location_bank.h"
#include "base/analysis/data/rules.h"
#include "base/analysis/data/transcript.h"
#include "base/analysis/data/entrant_data.h"
#include "base/analysis/scannable/scan_context.h"
#include "base/documents/data/date.h"
#include "base/documents/doc_class.h"
#include "base/documents/doc_type.h"

#include <optional>

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
			void SaveScannedDocuments(scannable::InspectionData& inspectionData);
			void HandleDocumentByType(documents::DocType documentType);

			void StoreBulletin(const documents::Doc& document);
			void StoreRuleBook(const documents::Doc& document);
			void StoreTranscript(const documents::Doc& document);

		private:  // Profiler:
			void AddToProfile(const documents::Doc& document);
			void ValidateDocument(const documents::Doc& document);
			// void CompareAgainstProfile(const documents::Doc& document);

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
			bool ValidateAgainstRulebook() const;
			bool ValidateAgainstTranscript() const;

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

#include "base/analysis/analysis_context.inl.h"
