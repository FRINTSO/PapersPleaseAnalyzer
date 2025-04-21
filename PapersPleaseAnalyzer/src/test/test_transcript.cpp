#include "pch.h"
#include "test/test_transcript.h"

#include "base/analysis/data/transcript.h"
#include "base/documents/doc_class.h"
#include "base/game_view.h"

namespace test {

    void test_transcript()
    {
        paplease::GameView game("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_57.png");
        auto transcriptDocOpt = paplease::documents::FindDocument(game, paplease::documents::DocType::Transcript);
        if (!transcriptDocOpt)
            assert(false && "Couldn't find a transcript!");

        const paplease::documents::Doc& transcriptDoc = transcriptDocOpt.value();

        auto transcriptOpt = paplease::analysis::data::CreateTranscript(transcriptDoc);
        if (!transcriptOpt)
            assert(false && "Couldn't extract transcript!");

        const paplease::analysis::data::Transcript& rulebook = transcriptOpt.value();
    }

}
