#pragma once
#include <array>
#include <optional>

#include "base/documents_v2/doc_class.h"


class DocumentState
{
public:
	void RegisterIfNewDocument(Documents::V2::Doc&& document);
	std::optional<std::reference_wrapper<const Documents::V2::Doc>> GetDocumentByType(const Documents::V2::DocType documentType) const;

	bool HasRuleBook() const;
	bool HasCriminals() const;

	void ClearAll();
	void ClearApplicantSession();
private:
	bool IsNewDocument(const Documents::V2::Doc& document) const;
	void RegisterDocument(Documents::V2::Doc&& document);
	bool HasDocumentOfType(const Documents::V2::DocType documentType) const;
private:
	static constexpr size_t DocumentCapacity = 1000;
private:

	std::array<Documents::V2::Doc, DocumentState::DocumentCapacity> m_documents;
	size_t m_documentCount = 9;
};