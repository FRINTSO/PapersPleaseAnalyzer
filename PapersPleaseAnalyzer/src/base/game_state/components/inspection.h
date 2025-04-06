#pragma once
#include <array>

#include "base/documents_v2/doc_class.h"
#include "base/documents_v2/doc_type.h"
#include "base/game_state/components/component.h"
#include "base/game_view.h"

namespace paplease {
	namespace analysis {
		namespace components {

			class InspectionComponent final : public Component
			{
			public:
				using Component::Component;

			public:
				static constexpr size_t DocumentScanCapacity = 4;
			public:
				std::array<documents::v2::Doc, InspectionComponent::DocumentScanCapacity> Scan(const GameView& gameView) const;

			public: // Mediator Funcs
				// void OnNewApplicant();
				// void AddDocumentToLookout(documents::v2::DocType documentType);

			public:
				static constexpr int InvalidIndex = -1;
			private:
				enum class DocumentState
				{
					Unknown,
					OnLookout,
					Found,
				};

			private:
				//bool TryRegisterIfApplicantDocument(const documents::v2::Doc& document);
				//void TrackDocuments(const GameView& gameView);

				//bool IsDocumentBeingLookedFor(documents::v2::DocType documentType) const;
				//void TryRemoveDocumentFromLookout(documents::v2::DocType documentType);
				//void NotifyLookoutDocumentFound(documents::v2::DocType documentType) const;
			private:

				static constexpr size_t ApplicantDocumentCapacity = 10;
				static constexpr size_t LookoutDocumentCapacity = 4;
			private:
				std::array<documents::v2::DocType, InspectionComponent::ApplicantDocumentCapacity> m_foundApplicantDocuments; // there can be max 10 docs
				size_t m_documentCount;
				std::array<documents::v2::DocType, InspectionComponent::LookoutDocumentCapacity> m_documentsToLookoutFor;
				size_t m_lookoutDocumentCount;
			};

		}  // namespace components
	}  // namespace analysis
}  // namespace paplease
