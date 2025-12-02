#include "pch.h"
#include "test/test_booth.h"

#include "paplease/game_view.h"
#include "paplease/scannable/booth.h"

namespace test {

    void test_booth_boxing()
    {
        paplease::GameView view("..\\Paplease-Core\\assets\\images\\game_44.png");

        auto data = paplease::scannable::ScanBooth(view);

    }

}  // namespace test
