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
				using DocRef = std::reference_wrapper<const documents::v2::Doc>;
			private:
				static constexpr size_t DocumentCapacity = 10;
			public:
				void RegisterIfNewDocument(documents::v2::Doc&& document);
				std::optional<DocRef> GetDocumentByType(const documents::v2::DocType documentType) const;
				const std::array<documents::v2::DocType, DocumentState::DocumentCapacity>& GetNewlyRegisteredTypes() const;
				void ClearNewlyRegistered();

				bool HasRuleBook() const;
				bool HasCriminals() const;

				void ClearAll();
				void ClearApplicantSession();
			private:
				bool IsNewDocument(const documents::v2::Doc& document) const;
				void RegisterDocument(documents::v2::Doc&& document);
				bool HasDocumentOfType(const documents::v2::DocType documentType) const;
				void AddToNewlyRegistered();
			private:

				std::array<documents::v2::Doc, DocumentState::DocumentCapacity> m_documents;
				size_t m_documentCount;

				std::array<documents::v2::DocType, DocumentState::DocumentCapacity> m_newRegistered;
				size_t m_newlyRegisteredCount;
			};

		}  // namespace components
	}  // namespace analysis
}  // namespace paplease
