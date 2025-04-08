#pragma once
#include <array>

#include "base/analysis/components/component.h"
#include "base/analysis/mediators/analysis_mediator.h"
#include "base/analysis/scannable/scan_context.h"
#include "base/documents/doc_data.h"

namespace paplease {
	namespace analysis {
		namespace components {

			class Profile
			{
			private:
				static constexpr size_t FieldsCapacity = 20;

			private:
				static std::array<documents::DataFieldCategory, Profile::FieldsCapacity> ProfileFields();
				static int GetFieldIndexByCategoryType(documents::DataFieldCategory category);

			private:
				std::array<documents::FieldData, Profile::FieldsCapacity> m_fields;
				size_t m_fieldCount;
			};

			class ApplicantState : public Component
			{
			public:
				using Component::Component;

			public: // Mediator functions
				void AddDocumentToProfile(const documents::Doc& document);
				void ResetProfile();

			private:
				void RegisterDocument(const documents::Doc& document);
			private:
				static constexpr size_t DocumentCapacity = 10;
			private:
				std::array<documents::Doc, ApplicantState::DocumentCapacity> m_comparableDocuments;
				size_t m_documentCount;

				Profile m_currentProfile;
			};

		}  // namespace components
	}  // namespace analysis
}  // namespace paplease
