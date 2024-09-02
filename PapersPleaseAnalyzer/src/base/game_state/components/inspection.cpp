#include "pch.h"
#include "base/game_state/components/inspection.h"

#include <iostream>
#include <utility>

#include "base/utils/log.h"

namespace paplease {
	namespace analysis {
		namespace components {

using namespace documents::v2;


std::array<Doc, InspectionComponent::DocumentScanCapacity> InspectionComponent::Scan(const GameView& gameView) const
{
	std::array<Doc, InspectionComponent::DocumentScanCapacity> foundDocuments;
	size_t count = 0;

	for (auto documentType : GetDocTypeIterator())
	{
		auto document = FindDocument(gameView, documentType);
		if (!document) continue;

		assert(count < InspectionComponent::DocumentScanCapacity);
		foundDocuments[count] = std::move(document.value());
		count++;
	}

	return foundDocuments;
}


#if OUTDATED
#pragma region Mediator Functions

void InspectionComponent::AddDocumentToLookout(DocType documentType)
{
	if (this->IsDocumentBeingLookedFor(documentType)) return;
	assert(m_lookoutDocumentCount != InspectionComponent::LookoutDocumentCapacity);

	for (auto& doc : m_documentsToLookoutFor)
	{
		if (doc == DocType::Invalid)
		{
			doc = documentType;
			return;
		}
	}
}

#pragma endregion

bool InspectionComponent::IsDocumentBeingLookedFor(DocType documentType) const
{
	for (DocType doc : m_documentsToLookoutFor)
	{
		if (doc == documentType)
		{
			return true;
		}
	}
	return false;
}

void InspectionComponent::TryRemoveDocumentFromLookout(DocType documentType)
{
	for (DocType& doc : m_documentsToLookoutFor)
	{
		if (doc == documentType)
		{
			doc = DocType::Invalid;
			return;
		}
	}
}

void InspectionComponent::NotifyLookoutDocumentFound(documents::v2::DocType documentType) const
{
	switch (documentType)
	{
		case documents::v2::DocType::RuleBook:
		{
			m_mediator->Notify(Sender::InspectionState, Event::RuleBookFound);
			break;
		}
		case documents::v2::DocType::Bulletin:
		{
			m_mediator->Notify(Sender::InspectionState, Event::RuleBookFound);
			break;
		}
		case documents::v2::DocType::Transcript:
		{
			m_mediator->Notify(Sender::InspectionState, Event::TranscriptFound);
			break;
		}
		default:
		{
			LOG_ERR("Document type cannot be notified!");
			break;
		}
	}
}

void InspectionComponent::OnNewApplicant()
{ // Clear Docs
	for (auto& documentType : m_foundApplicantDocuments)
	{
		documentType = DocType::Invalid;
	}
	m_documentCount = 0;
}

#pragma region Conversion

static constexpr int ApplicantDocTypeToIndex(DocType documentType)
{
	switch (documentType)
	{
		case DocType::AccessPermit:
			return 0;
		case DocType::CertificateOfVaccination:
			return 1;
		case DocType::DiplomaticAuthorization:
			return 2;
		case DocType::EntryPermit:
			return 3;
		case DocType::EntryTicket:
			return 4;
		case DocType::GrantOfAsylum:
			return 5;
		case DocType::IdentityCard:
			return 6;
		case DocType::IdentitySupplement:
			return 7;
		case DocType::WorkPass:
			return 8;
		case DocType::Passport:
			return 9;
		default:
			return InspectionComponent::InvalidIndex;
	}
}

static constexpr int DocTypeToIndex(DocType documentType)
{
	return ApplicantDocTypeToIndex(documentType);
}

static constexpr DocType IndexToDocType(int index)
{
	if (index == -1) return DocType::Invalid;
	auto documentType = static_cast<DocType>(index + 1);
	return documentType;
}

#pragma endregion


bool InspectionComponent::TryRegisterIfApplicantDocument(const Doc& document)
{
	if (!IsApplicantDocument(document.GetDocumentType())) return false;

	auto documentIndex = DocTypeToIndex(document.GetDocumentType());
	if (documentIndex == -1) return false;

	if (m_foundApplicantDocuments[documentIndex] == DocType::Invalid)
	{
		m_mediator->NotifyDocumentFound(document);
		m_foundApplicantDocuments[documentIndex] = document.GetDocumentType();
		return true;
	}
	else
	{
		LOG("Document position is occupied!");
		return false;
	}
}

void InspectionComponent::TrackDocuments(const GameView& gameView)
{
	
	for (auto documentType : GetDocTypeIterator())
	{
		auto document = FindDocument(gameView, documentType);
		if (!document) continue;

		// Should be on lookout for needed documents
		// ex. transcript, criminals, rulebook
		if (this->IsDocumentBeingLookedFor(documentType))
		{
			this->TryRemoveDocumentFromLookout(documentType);
			m_mediator->NotifyDocumentFound(document.value());
		}

		this->TryRegisterIfApplicantDocument(document.value());
		
	}
}

#endif

		}  // namespace components
	}  // namespace analysis
}  // namespace paplease
