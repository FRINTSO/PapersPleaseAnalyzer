#include "pch.h"
#include "base/analysis/components/document_state.h"

#include "base/utils/log.h"

#include <utility>

namespace paplease {
	namespace analysis {
		namespace components {

			using namespace documents;

			void DocumentState::Update(scannable::ScanContext& scanContext)
			{
				BeginLOG("DocumentState::Update()");
				if (!scanContext.inspectionData)
				{
					LOG("Inspection-view had no documents.");
					EndLOG("DocumentState::Update()");
					return;
				}
				
				for (auto& document : scanContext.inspectionData->documents)
				{
					if (document.GetDocumentType() == documents::DocType::Invalid)
					{
						break;
					}
					this->RegisterIfNewDocument(std::move(document));
				}

				EndLOG("DocumentState::Update()");
			}

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

				LOG("WOW, new {} registered", ToStringView(document.GetDocumentType()));

				// Add
				auto docType = document.GetDocumentType();
				this->RegisterDocument(std::move(document));
				this->AddToNewlyRegistered();

				m_mediator->NotifyDocumentFound(this->GetDocumentByType(docType).value());
			}

			const std::array<DocType, DocumentState::DocumentCapacity>& DocumentState::GetNewlyRegisteredTypes() const
			{
				return m_newRegistered;
			}

			void DocumentState::AddToNewlyRegistered()
			{
				// Add most recent document to array
				m_newRegistered[m_newlyRegisteredCount] = m_documents[m_documentCount - 1].GetDocumentType();
				m_newlyRegisteredCount++;
			}

			void DocumentState::ClearNewlyRegistered()
			{
				for (auto& newDocument : m_newRegistered)
				{
					newDocument = DocType::Invalid;
				}
				m_newlyRegisteredCount = 0;
			}

			std::optional<DocumentState::DocRef> DocumentState::GetDocumentByType(const documents::DocType documentType) const
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
			{  // This code should be tested
				for (size_t i = 0; i < m_documentCount; i++)
				{
					if (components::IsApplicantDocument(m_documents[i].GetDocumentType()))
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

				// Scuffed
				m_documentCount = 0;
				for (const auto& document : m_documents)
				{
					if (document.GetDocumentType() != DocType::Invalid)
					{
						m_documentCount++;
					}
				}
			}

			bool DocumentState::IsNewDocument(const documents::Doc& document) const
			{
				return !this->HasDocumentOfType(document.GetDocumentType());
			}

			void DocumentState::RegisterDocument(Doc&& document)
			{
				assert(m_documentCount < DocumentState::DocumentCapacity);

				m_documents[m_documentCount] = std::move(document);
				m_documentCount++;
			}

			bool DocumentState::HasDocumentOfType(const documents::DocType documentType) const
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

		}  // namespace components
	}  // namespace analysis
}  // namespace paplease
