#pragma once
#include "paplease/core/fixed.h"
#include "paplease/documents/doc_type.h"
#include "paplease/game_view.h"

namespace paplease {
    namespace analysis_v1 {

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

    }  // namespace analysis_v1
}  // namespace paplease
