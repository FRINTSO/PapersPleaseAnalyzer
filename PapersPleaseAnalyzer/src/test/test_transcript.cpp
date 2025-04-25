#include "pch.h"
#include "test/test_transcript.h"

#include "paplease/analysis/data/transcript.h"
#include "paplease/documents/doc_class.h"
#include "paplease/scannable/doc_scan.h"
#include "paplease/game_view.h"

namespace test {

    void test_transcript()
    {
        paplease::GameView game("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_57.png");
        auto transcriptDocOpt = paplease::scannable::ScanForDocument(game, paplease::ViewArea::InspectionView, paplease::documents::DocType::Transcript);
        if (!transcriptDocOpt)
            assert(false && "Couldn't find a transcript!");

        auto transcriptDoc = transcriptDocOpt->ToDocument(game);

        auto transcriptOpt = paplease::analysis::data::CreateTranscript(transcriptDoc);
        if (!transcriptOpt)
            assert(false && "Couldn't extract transcript!");

        const paplease::analysis::data::Transcript& rulebook = transcriptOpt.value();
    }

}  // namespace test
