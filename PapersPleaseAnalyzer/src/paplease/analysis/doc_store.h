#pragma once
#include "paplease/core/fixed.h"
#include "paplease/documents/doc_class.h"
#include "paplease/documents/doc_data.h"
#include "paplease/documents/doc_view.h"


namespace paplease {
    namespace analysis {

        struct DocAnalysisStatus
        {
            // Requirements
            bool isRequired = false;  // Document is expected based on current rules
            bool isRequiredReported = false;

            // Processing state
            bool wasScanned = false;  // Document has been detected on screen
            bool wasValidated = false;  // Data has been successfully extracted from the document
            bool wasAnalyzed = false;  // Document content has been analyzed for rule compliance

            // Conflict status
            bool hasConflict = false;  // A discrepancy or issue was found in the document
            bool conflictCleared = false;  // The discrepancy has been resolved or cleared
        };

        struct DocRequirement
        {
            documents::DocType documentType;
            documents::PassportType passportType = documents::PassportType::Invalid;

            constexpr bool operator==(const DocRequirement& other) const noexcept
            {
                return documentType == other.documentType &&
                    passportType == other.passportType;
            }
        };

        namespace detail {

            struct HashPair
            {
                template <typename T, typename U>
                std::size_t operator()(const std::pair<T, U>& p) const
                {
                    auto h1 = std::hash<T>{}(p.first);  // Hash the first element
                    auto h2 = std::hash<U>{}(p.second); // Hash the second element

                    // Combine the hashes using a standard method
                    return h1 ^ (h2 << 1);  // Simple XOR and shift combination
                }
            };

            struct DocumentRequirementHash
            {
                std::size_t operator()(const DocRequirement& r) const noexcept
                {
                    return HashPair{}(std::pair{ r.documentType, r.passportType });
                }
            };

        }  // namespace detail

        // Class for storing documents
        class DocStore
        {
        private:
            template<typename ValueType>
            using DocHashTable = core::FixedHashTable<documents::DocType, ValueType, static_cast<size_t>(documents::DocType::Count)>;

        public:
            const documents::DocView& StoreDocumentView(documents::DocView&& documentView);
            const documents::Doc& StoreDocument(documents::Doc&& document);
            const documents::DocData& StoreDocumentData(documents::DocType documentType, documents::DocData&& documentData);


            template<typename DocClass>
            constexpr const DocHashTable<DocClass>& GetStoredCollection() const
            {
                if constexpr (std::is_same_v<DocClass, documents::DocView>)
                {
                    return m_storedDocumentViews;
                }
                else if constexpr (std::is_same_v<DocClass, documents::Doc>)
                {
                    return m_storedDocuments;
                }
                else if constexpr (std::is_same_v<DocClass, documents::DocData>)
                {
                    return m_storedDocumentData;
                }
                else
                {
                    static_assert(false);
                }
            }
            template<typename DocClass>
            constexpr DocHashTable<DocClass>& GetStoredCollection()
            {
                if constexpr (std::is_same_v<DocClass, documents::DocView>)
                {
                    return m_storedDocumentViews;
                }
                else if constexpr (std::is_same_v<DocClass, documents::Doc>)
                {
                    return m_storedDocuments;
                }
                else if constexpr (std::is_same_v<DocClass, documents::DocData>)
                {
                    return m_storedDocumentData;
                }
                else
                {
                    static_assert(false);
                }
            }
            template<typename DocClass>
            constexpr const DocClass& GetStored(documents::DocType documentType) const
            {
                if constexpr (std::is_same_v<DocClass, documents::DocView>)
                {
                    return m_storedDocumentViews[documentType];
                }
                else if constexpr (std::is_same_v<DocClass, documents::Doc>)
                {
                    return m_storedDocuments[documentType];
                }
                else if constexpr (std::is_same_v<DocClass, documents::DocData>)
                {
                    return m_storedDocumentData[documentType];
                }
                else
                {
                    static_assert(false);
                }
            }
            template<typename DocClass>
            constexpr DocClass& GetStored(documents::DocType documentType)
            {
                if constexpr (std::is_same_v<DocClass, documents::DocView>)
                {
                    return m_storedDocumentViews[documentType];
                }
                else if constexpr (std::is_same_v<DocClass, documents::Doc>)
                {
                    return m_storedDocuments[documentType];
                }
                else if constexpr (std::is_same_v<DocClass, documents::DocData>)
                {
                    return m_storedDocumentData[documentType];
                }
                else
                {
                    static_assert(false);
                }
            }
            template<typename DocClass>
            constexpr bool Contains(documents::DocType documentType) const
            {
                if constexpr (std::is_same_v<DocClass, documents::DocView>)
                {
                    return m_storedDocumentViews.Contains(documentType);
                }
                else if constexpr (std::is_same_v<DocClass, documents::Doc>)
                {
                    return m_storedDocuments.Contains(documentType);
                }
                else if constexpr (std::is_same_v<DocClass, documents::DocData>)
                {
                    return m_storedDocumentData.Contains(documentType);
                }
                else
                {
                    static_assert(false);
                }
            }

            DocAnalysisStatus& GetAnalysisStatus(documents::DocType documentType);
            const core::FixedHashSet<DocRequirement, static_cast<size_t>(documents::DocType::Count), detail::DocumentRequirementHash>&
                GetRequiredTypes() const noexcept;

            void SetRequiredType(DocRequirement requiredType);

            void Clear()
            {
                // Store rulebook
                auto type = documents::DocType::RuleBook;
                documents::DocView* pdocView = nullptr;
                documents::Doc* pdoc = nullptr;
                documents::DocData* pdocData = nullptr;
                m_storedDocumentViews.Get(type, pdocView);
                m_storedDocuments.Get(type, pdoc);
                m_storedDocumentData.Get(type, pdocData);
                auto& docStatus = m_storedDocumentAnalysisStatus.Get(type);

                //if (pdocView)
                //    auto docView = *pdocView;
                //if (pdoc)
                //    auto doc = *pdoc;

                //if (pdocData)
                //    auto docData = *pdocData;

                // Clear
                m_storedDocumentViews.Clear();
                m_storedDocuments.Clear();
                m_storedDocumentData.Clear();
                m_storedDocumentAnalysisStatus.Clear();
                m_requiredDocuments.Clear();

                // Add rulebook
                if (pdocView)
                    m_storedDocumentViews.Set(type, *pdocView);
                if (pdoc)
                    m_storedDocuments.Set(type, *pdoc);
                if (pdocData)
                    m_storedDocumentData.Set(type, *pdocData);
                m_storedDocumentAnalysisStatus.Set(type, docStatus);
            }

        private:
            DocHashTable<documents::DocView> m_storedDocumentViews{};
            DocHashTable<documents::Doc> m_storedDocuments{};
            DocHashTable<documents::DocData> m_storedDocumentData{};
            
            core::FixedTable<documents::DocType, DocAnalysisStatus> m_storedDocumentAnalysisStatus{};
            core::FixedHashSet<DocRequirement, static_cast<size_t>(documents::DocType::Count), detail::DocumentRequirementHash> m_requiredDocuments{};
        };

    }  // namespace analysis
}  // namespace paplease
