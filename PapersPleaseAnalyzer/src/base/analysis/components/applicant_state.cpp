#include "pch.h"
#include "base/analysis/components/applicant_state.h"

#include "base/utils/log.h"

namespace paplease {
	namespace analysis {
		namespace components {

			using namespace documents;

			void ApplicantState::AddDocumentToProfile(const documents::Doc& document)
			{
				// Check document authenticity
				if (!document.IsAuthentic())
				{
					LOG_ERR("Document has an invalid seal.");
				}

				this->RegisterDocument(document);
			}
			
			void ApplicantState::ResetProfile()
			{
				for (auto& document : m_comparableDocuments)
				{
					document = {};
				}
				m_documentCount = 0;
			}

			void ApplicantState::RegisterDocument(const documents::Doc& document)
			{
				assert(m_documentCount + 1 < ApplicantState::DocumentCapacity);

				LOG("Registered a document to Profiler!");

				m_comparableDocuments[m_documentCount] = document;
				m_documentCount++;
			}

			std::array<FieldCategory, Profile::FieldsCapacity> Profile::ProfileFields()
			{
				// Choose what fields should be used
				return {
					FieldCategory::DateOfBirth,
					FieldCategory::Height,
					FieldCategory::IssuingCity,
					FieldCategory::IssuingCountry,
					FieldCategory::Name,
					FieldCategory::PassportNumber,
					FieldCategory::Sex,
					FieldCategory::Vaccination1,
					FieldCategory::Vaccination2,
					FieldCategory::Vaccination3,
					FieldCategory::Weight,
				};
			}

			int Profile::GetFieldIndexByCategoryType(documents::FieldCategory category)
			{
				switch (category)
				{
					case FieldCategory::DateOfBirth:
						return 0;
					case FieldCategory::Height:
						return 1;
					case FieldCategory::IssuingCity:
						return 2;
					case FieldCategory::IssuingCountry:
						return 3;
					case FieldCategory::Name:
						return 4;
					case FieldCategory::PassportNumber:
						return 5;
					case FieldCategory::Sex:
						return 6;
					case FieldCategory::Vaccination1:
						return 7;
					case FieldCategory::Vaccination2:
						return 8;
					case FieldCategory::Vaccination3:
						return 9;
					case FieldCategory::Weight:
						return 10;
					default:
						return -1;
				}
			}

		}  // namespace components
	}  // namespace analysis
}  // namespace paplease
