#pragma once
#include <opencv2/opencv.hpp>

#include "paplease/analysis/analysis_context_v2.h"
#include "paplease/analysis/doc_store.h"
#include "paplease/core/fixed.h"
#include "paplease/documents/doc_type.h"
#include "paplease/game_view.h"
#include "paplease/scannable/scan_context.h"

namespace paplease {
    namespace analysis {

        struct TrackedDocument
        {
            documents::DocType documentType;
            cv::Rect box;

            ViewArea area;
        };

        class DocTracker
        {
        public:
            DocTracker();

            void Update(const GameView& gameView);
        public:
            void OnNewDate();
            void OnNewApplicant();
        private:
            void TrackDocsInBoothView(const GameView& view);
            void TrackDocsInInspectionView(const GameView& view);

        private:
            core::FixedTable<documents::DocType, TrackedDocument> m_documentPositions;
            core::FixedArray<documents::DocType, static_cast<size_t>(documents::DocType::Count)> m_documentsInBoothView;
        };

        


        // Tracks document position and 
        class DocTrackerV2
        {
        private:
            static inline constexpr size_t DocTypeCount = static_cast<size_t>(documents::DocType::Count);
            using DocTypeSet = core::FixedHashSet<documents::DocType, DocTypeCount>;
        public:
            constexpr DocTrackerV2(DocStore& store, AnalysisContextV2& analysisContext)
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
            AnalysisContextV2& m_context;
            
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
