#include "pch.h"
#include "test/rulebook.h"

#include "base/analysis/data/rules.h"
#include "base/documents/doc_class.h"
#include "base/game_view.h"

namespace test {

    void test_rulebook()
    {
        paplease::GameView game("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_sim\\3\\game_4.png");
        auto rulebookDocOpt = paplease::documents::FindDocument(game, paplease::documents::DocType::RuleBook);
        if (!rulebookDocOpt)
            assert(false && "Couldn't find a rulebook!");

        const paplease::documents::Doc& rulebookDoc = rulebookDocOpt.value();

        auto rulebookOpt = paplease::analysis::data::CreateRuleBook(rulebookDoc);
        if (!rulebookOpt)
            assert(false && "Couldn't extract rulebook!");

        const paplease::analysis::data::RuleBook& rulebook = rulebookOpt.value();

        
    }

}  // namespace test
