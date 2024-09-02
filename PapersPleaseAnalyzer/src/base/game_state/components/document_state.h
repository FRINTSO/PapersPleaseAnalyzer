#pragma once
#include <array>
#include <optional>
#include <type_traits>

#include "base/documents_v2/doc_class.h"
#include "base/documents_v2/doc_type.h"

namespace paplease {
	namespace analysis {
		namespace components {

			class DocumentState
			{
			public:
				void RegisterIfNewDocument(documents::v2::Doc&& document);
				std::optional<std::reference_wrapper<const documents::v2::Doc>> GetDocumentByType(const documents::v2::DocType documentType) const;

				bool HasRuleBook() const;
				bool HasCriminals() const;

				void ClearAll();
				void ClearApplicantSession();
			private:
				bool IsNewDocument(const documents::v2::Doc& document) const;
				void RegisterDocument(documents::v2::Doc&& document);
				bool HasDocumentOfType(const documents::v2::DocType documentType) const;
			private:
				static constexpr size_t DocumentCapacity = 15;
			private:

				std::array<documents::v2::Doc, DocumentState::DocumentCapacity> m_documents;
				size_t m_documentCount = 9;
			};

		}  // namespace components
	}  // namespace analysis
}  // namespace paplease
