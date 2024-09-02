#include "pch.h"
#include "base/game_state/document_state.h"

#include "base/utils/log.h"

#include <utility>

using namespace Documents::V2;

static inline constexpr bool IsApplicantDocument(const DocType documentType)
{
	switch (documentType)
	{
		case DocType::AccessPermit:
		case DocType::CertificateOfVaccination:
		case DocType::DiplomaticAuthorization:
		case DocType::EntryPermit:
		case DocType::EntryTicket:
		case DocType::GrantOfAsylum:
		case DocType::IdentityCard:
		case DocType::IdentitySupplement:
		case DocType::WorkPass:
		case DocType::Passport:
		case DocType::Transcript:
			return true;
		default:
			return false;
	}
}

void DocumentState::RegisterIfNewDocument(Doc&& document)
{
	if (!this->IsNewDocument(document)) return;

	LOG("WOW, new document registered");

	// Add
	this->RegisterDocument(std::move(document));
}

std::optional<std::reference_wrapper<const Documents::V2::Doc>> DocumentState::GetDocumentByType(const Documents::V2::DocType documentType) const
{
	for (size_t i = 0; i < m_documentCount; i++)
	{
		if (m_documents[i].GetDocumentType() == documentType)
		{
			return m_documents[i];
		}
	}
	return std::nullopt;
}

bool DocumentState::HasRuleBook() const
{
	return HasDocumentOfType(DocType::RuleBook);
}

bool DocumentState::HasCriminals() const
{
	return HasDocumentOfType(DocType::Bulletin);
}

void DocumentState::ClearAll()
{
	for (size_t i = 0; i < m_documentCount; i++)
	{
		m_documents[i] = {};
	}
	m_documentCount = 0;
}

void DocumentState::ClearApplicantSession()
{
	for (size_t i = 0; i < m_documentCount; i++)
	{
		if (::IsApplicantDocument(m_documents[i].GetDocumentType()))
		{
			m_documents[i] = {};
		}
	}

	int openSlot = -1;
	for (size_t i = 0; i < m_documentCount; i++)
	{
		if (m_documents[i].GetDocumentType() == DocType::Invalid)
		{
			openSlot = i;
		}
		else
		{
			if (openSlot != -1)
			{
				m_documents[openSlot] = std::move(m_documents[i]);
				openSlot = -1;
			}
		}
	}
}

bool DocumentState::IsNewDocument(const Documents::V2::Doc& document) const
{
	return !this->HasDocumentOfType(document.GetDocumentType());
}

void DocumentState::RegisterDocument(Doc&& document)
{
	assert(m_documentCount < DocumentState::DocumentCapacity);

	m_documents[m_documentCount] = std::move(document);
	m_documentCount++;
}

bool DocumentState::HasDocumentOfType(const Documents::V2::DocType documentType) const
{
	for (size_t i = 0; i < m_documentCount; i++)
	{
		if (m_documents[i].GetDocumentType() == documentType)
		{
			return true;
		}
	}
	return false;
}