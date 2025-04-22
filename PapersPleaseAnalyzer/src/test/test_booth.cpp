#include "pch.h"
#include "test/test_booth.h"

#include "base/game_view.h"
#include "base/analysis/scannable/booth.h"

namespace test {

    void test_booth_boxing()
    {
        paplease::GameView view("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_44.png");

        auto data = paplease::analysis::scannable::ScanBooth(view);

    }

}  // namespace test
