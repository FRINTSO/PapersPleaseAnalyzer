#include "pch.h"
#include "base/game_state/states/inspection_state.h"

#include <iostream>
#include <utility>

#include "base/utils/log.h"

using namespace Documents::V2;

void InspectionState::Update(const GameView& gameView)
{
	BeginLOG(InspectionState::Update);

	// check if new applicant

	// Update passports
	// Cheap implementation
	this->TrackDocuments(gameView);



	EndLOG(InspectionState::Update);
}


#pragma region Mediator Functions

void InspectionState::AddDocumentToLookout(DocType documentType)
{
	if (this->IsDocumentBeingLookedFor(documentType)) return;
	assert(m_lookoutDocumentCount != InspectionState::LookoutDocumentCapacity);

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

bool InspectionState::IsDocumentBeingLookedFor(DocType documentType) const
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

void InspectionState::TryRemoveDocumentFromLookout(DocType documentType)
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

void InspectionState::NotifyLookoutDocumentFound(Documents::V2::DocType documentType) const
{
	switch (documentType)
	{
		case Documents::V2::DocType::RuleBook:
		{
			m_mediator->Notify(Sender::InspectionState, Event::RuleBookFound);
			break;
		}
		case Documents::V2::DocType::Bulletin:
		{
			m_mediator->Notify(Sender::InspectionState, Event::RuleBookFound);
			break;
		}
		case Documents::V2::DocType::Transcript:
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

void InspectionState::OnNewApplicant()
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
			return InspectionState::InvalidIndex;
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


bool InspectionState::TryRegisterIfApplicantDocument(const Doc& document)
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

void InspectionState::TrackDocuments(const GameView& gameView)
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