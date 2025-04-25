#pragma once
#include <opencv2/opencv.hpp>

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

        class DocTrackerV2
        {
        public:
            DocTrackerV2() = default;

            void RefreshTracking(const scannable::ScanContext& scanContext);

        private:
            void RegisterScannedDocuments(const scannable::DocViewCollection& scannedDocuments);

        private:
            core::FixedTable<documents::DocType, documents::DocView> m_documents{  };
            core::FixedHashSet<documents::DocType, static_cast<size_t>(documents::DocType::Count)> m_documentsNotVisible{};
        };

    }  // namespace analysis
}  // namespace paplease
