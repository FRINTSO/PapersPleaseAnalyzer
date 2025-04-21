#pragma once
#include <opencv2/opencv.hpp>

#include "base/game_view.h"

namespace paplease {
    namespace analysis {
        namespace scannable {

            class DocTracker
            {
            public:
                void Update(const GameView& gameView);

            private:
                void TrackDocsInBoothView(const GameView& view);
                void TrackDocsInInspectionView(const GameView& view);
            };

        }
    }
}
