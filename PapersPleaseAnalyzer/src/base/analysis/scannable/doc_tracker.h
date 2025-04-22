#pragma once
#include <opencv2/opencv.hpp>

#include "base/documents/doc_type.h"
#include "base/game_view.h"
#include "base/shape.h"
#include "base/utils/fixed_array.h"
#include "base/utils/table.h"

namespace paplease {
    namespace analysis {
        namespace scannable {

            enum class ViewArea : unsigned char
            {
                Invalid,
                Booth,
                Inspection
            };

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
                utils::FixedTable<documents::DocType, TrackedDocument> m_documentPositions;

                utils::FixedArray<documents::DocType, static_cast<size_t>(documents::DocType::Count)> m_documentsInBoothView;
            };

        }
    }
}
