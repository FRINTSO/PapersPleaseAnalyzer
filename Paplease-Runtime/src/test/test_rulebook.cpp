#include "pch.h"
#include "test/test_rulebook.h"

#include "paplease/analysis/data/rules.h"
#include "paplease/documents/doc_class.h"
#include "paplease/scannable/doc_scan.h"
#include "paplease/game_view.h"

namespace test {

    void test_rulebook()
    {
        paplease::GameView game("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_sim\\3\\game_4.png");
        auto rulebookDocOpt = paplease::scannable::ScanForDocument(game, paplease::ViewArea::InspectionView, paplease::documents::DocType::RuleBook);
        if (!rulebookDocOpt)
            assert(false && "Couldn't find a rulebook!");

        const paplease::documents::Doc& rulebookDoc = rulebookDocOpt->ToDocument(game);

        auto rulebookOpt = paplease::analysis::data::CreateRuleBook(rulebookDoc);
        if (!rulebookOpt)
            assert(false && "Couldn't extract rulebook!");

        const paplease::analysis::data::RuleBook& rulebook = rulebookOpt.value();

        
    }

}  // namespace test
