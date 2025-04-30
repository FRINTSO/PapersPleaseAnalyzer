#include "pch.h"
#include "test/test_transcript.h"

#include "paplease/analysis/data/transcript.h"
#include "paplease/documents/doc_class.h"
#include "paplease/scannable/doc_scan.h"
#include "paplease/game_view.h"

namespace test {

    using namespace paplease;
    using namespace documents;
    using namespace scannable;
    using namespace analysis;
    using namespace analysis::data;

    void test_transcript()
    {
        GameView game("C:\\dev\\PapersPleaseAnalyzer\\PapersPleaseAnalyzer\\images\\game_59.png");
        auto transcriptDocOpt = ScanForDocument(game, ViewArea::InspectionView, DocType::Transcript);
        if (!transcriptDocOpt)
            assert(false && "Couldn't find a transcript!");

        auto transcriptDoc = transcriptDocOpt->ToDocument(game);

        auto transcriptOpt = CreateTranscript(transcriptDoc);
        if (!transcriptOpt)
            assert(false && "Couldn't extract transcript!");

        const Transcript& rulebook = transcriptOpt.value();

        for (auto& entry : rulebook.Entries())
        {
            LOG("{}: \"{}\"",
                magic_enum::enum_name(entry.speakerRole),
                entry.dialogueLine);
        }
    }

}  // namespace test
