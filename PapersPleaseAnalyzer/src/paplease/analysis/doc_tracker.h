#pragma once
#include "paplease/analysis/analysis_context.h"
#include "paplease/analysis/doc_store.h"
#include "paplease/core/fixed.h"
#include "paplease/documents/doc_type.h"
#include "paplease/scannable/scan_context.h"

namespace paplease {
    namespace analysis {

        // Tracks document position and 
        class DocTracker
        {
        private:
            static inline constexpr size_t DocTypeCount = static_cast<size_t>(documents::DocType::Count);
            using DocTypeSet = core::FixedHashSet<documents::DocType, DocTypeCount>;
        public:
            constexpr DocTracker(DocStore& store, AnalysisContext& analysisContext)
                : m_store(store), m_context(analysisContext) {}

            void RefreshTracking(const scannable::ScanContext& scanContext);

            // Get documents that have changed in appearance since last scan
            DocTypeSet GetVisibleDocuments() const;

            DocTypeSet GetUpdatedDocuments() const;


            void AddRequiredDocument(std::pair<documents::DocType, documents::PassportType> docType);

            void UpdateRequiredDocuments();

            void ReportMissingDocuments() const noexcept;

        private:
            void RegisterScannedDocuments(const scannable::DocViewCollection& scannedDocuments);
            void RegisterSingleScannedDocument(documents::DocView&& scannedDocView);
            
        private:
            DocStore& m_store;
            AnalysisContext& m_context;
            
            core::FixedHashSet<documents::DocType, DocTypeCount> m_visibleDocuments{};
            //core::FixedHashTable<
            //    std::pair<
            //        documents::DocType,
            //        documents::PassportType
            //    >,
            //    bool,
            //    DocTypeCount,
            //    detail::HashPair
            //> m_requiredDocuments;
        };  

    }  // namespace analysis
}  // namespace paplease
