#include "pch.h"
#include "paplease/analysis/doc_store.h"

#include <magic_enum/magic_enum.hpp>

namespace paplease {
    namespace analysis {

        const documents::DocView& DocStore::StoreDocumentView(documents::DocView&& documentView)
        {
            const documents::DocType documentType = ToDocType(documentView.appearanceType);
            if (m_storedDocumentViews.Contains(documentType))
            {
                LOG_WARN(
                    "Storing documentView[{}] where one already exists! Overwriting the old with the new.",
                    magic_enum::enum_name(documentType)
                );
            }

            const bool success = m_storedDocumentViews.Set(documentType, std::move(documentView));
            assert(success, "Could not store document view in DocStore!");

            return m_storedDocumentViews[documentType];
        }

        const documents::Doc& DocStore::StoreDocument(documents::Doc&& document)
        {
            const documents::DocType documentType = document.GetDocumentType();
            if (m_storedDocuments.Contains(documentType))
            {
                LOG_WARN(
                    "Storing a document[{}] where one already exists! Overwriting the old with the new.",
                    magic_enum::enum_name(documentType)
                );
            }

            const bool success = m_storedDocuments.Set(documentType, std::move(document));
            assert(success, "Could not store document in DocStore!");

            return m_storedDocuments[documentType];
        }

        const documents::DocData& DocStore::StoreDocumentData(documents::DocType documentType, documents::DocData&& documentData)
        {
            if (m_storedDocumentData.Contains(documentType))
            {
                LOG_WARN(
                    "Storing documentData[{}] where one already exists! Overwriting the old with the new.",
                    magic_enum::enum_name(documentType)
                );
            }

            const bool success = m_storedDocumentData.Set(documentType, std::move(documentData));
            assert(success, "Could not store DocumentData in DocStore!");

            return m_storedDocumentData[documentType];
        }

        DocAnalysisStatus& DocStore::GetAnalysisStatus(documents::DocType documentType)
        {
            return m_storedDocumentAnalysisStatus.Get(documentType);
        }

        const core::FixedHashSet<DocRequirement, static_cast<size_t>(documents::DocType::Count), detail::DocumentRequirementHash>&
            DocStore::GetRequiredTypes() const noexcept
        {
            return m_requiredDocuments;
        }

        void DocStore::SetRequiredType(DocRequirement requiredType)
        {
            m_requiredDocuments.Insert(requiredType);
        }

    }  // namespace analysis
}  // namespace paplease
