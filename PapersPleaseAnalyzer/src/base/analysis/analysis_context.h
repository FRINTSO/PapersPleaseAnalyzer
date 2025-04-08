#pragma once
#include "base/analysis/data/criminals.h"
#include "base/analysis/data/rules.h"
#include "base/analysis/data/transcript.h"
#include "base/analysis/scannable/scan_context.h"
#include "base/document_data/date.h"
#include "base/documents/doc_class.h"

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
			void OnNewDate();
			void OnNewApplicant();
		private:
			void Clear();

			static constexpr size_t FieldsCapacity = 20;
			static constexpr std::array<documents::DataFieldCategory, FieldsCapacity> ProfileFields();
			static constexpr int GetFieldIndexByCategoryType(documents::DataFieldCategory category);

		private:
			std::array<documents::FieldData, Profile::FieldsCapacity> m_fields;
			size_t m_fieldCount;
		};

		class AnalysisContext  // Object containing everything necessary for any analysis to take place
		{
		public:
			void Update(scannable::ScanContext& scanContext);

		private:
			bool SetIfNewDate(documents::data::Date date);
			bool SetIfNewApplicant(int applicantNumber);

			void OnNewDate();
			void OnNewApplicant();

		private: // DocRegistry:
			void HandleInspectionData(scannable::InspectionData& inspectionData);
			void HandleDocumentByType(documents::DocType documentType);

			void StoreBulletin(const documents::Doc& document);
			void StoreRuleBook(const documents::Doc& document);
			void StoreTranscript(const documents::Doc& document);

		private:  // Profiler:
			void AddToProfile(const documents::Doc& document);
			void ValidateDocument(const documents::Doc& document);
			void ValidateField(const documents::FieldData& field);
			// void CompareAgainstProfile(const documents::Doc& document);

		private:
			// Plain data
			documents::data::Date m_currentDate;
			int m_applicantNumber;

			std::optional<data::RuleBook> m_ruleBook;
			std::optional<data::CriminalData> m_criminalData;
			std::optional<data::Transcript> m_transcript;

			// More complex
			DocRegistry m_docRegistry;  // stores all documents currently in effect

			// Profiler:
			Profile m_profile;
		};

		class DocumentValidator
		{
		public:
			DocumentValidator(const documents::Doc& document);

			bool Validate();

			bool ValidateAuthenticity() const;
			bool ValidateExpiryDate() const;
			bool ValidateNationality() const;
			bool ValidateDateOfBirth() const;
			bool ValidateVaccineData() const;
			bool ValidateField(const documents::FieldData& field) const;

		private:
			const documents::Doc& m_document;  // The document being validated
			documents::DocData m_documentData;
		};

	}  // namespace analysis
}  // namespace paplease

#include "base/analysis/analysis_context.inl.h"
