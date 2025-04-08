#pragma once
#include <array>
#include <optional>
#include <type_traits>

#include "base/analysis/components/component.h"
#include "base/analysis/scannable/scan_context.h"
#include "base/documents/doc_class.h"
#include "base/documents/doc_type.h"

namespace paplease {
	namespace analysis {
		namespace components {

			class DocumentState : public Component
			{
			public:
				using Component::Component;

				void Update(scannable::ScanContext& scanContext);
			public:
				using DocRef = std::reference_wrapper<const documents::Doc>;
			private:
				static constexpr size_t DocumentCapacity = 10;
			public:
				void RegisterIfNewDocument(documents::Doc&& document);
				std::optional<DocRef> GetDocumentByType(const documents::DocType documentType) const;
				const std::array<documents::DocType, DocumentState::DocumentCapacity>& GetNewlyRegisteredTypes() const;
				void ClearNewlyRegistered();

				bool HasRuleBook() const;
				bool HasCriminals() const;

				void ClearAll();
				void ClearApplicantSession();
			private:
				bool IsNewDocument(const documents::Doc& document) const;
				void RegisterDocument(documents::Doc&& document);
				bool HasDocumentOfType(const documents::DocType documentType) const;
				void AddToNewlyRegistered();
			private:

				std::array<documents::Doc, DocumentState::DocumentCapacity> m_documents;
				size_t m_documentCount;

				std::array<documents::DocType, DocumentState::DocumentCapacity> m_newRegistered;
				size_t m_newlyRegisteredCount;
			};

		}  // namespace components
	}  // namespace analysis
}  // namespace paplease
