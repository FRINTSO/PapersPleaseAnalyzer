#include "frame_observer.h"
#include <paplease/vision.h>
#include <paplease/documents.h>
#include "rulebook.h"

observation observe_frame(const game_screen& screen, const resources_ctx& ctx) {
    observation obs;

    // 1. booth info
    booth_info booth;
    obs.booth_readable = extract_booth_info(booth, screen, ctx);
    if (obs.booth_readable) {
        obs.date = booth.current_date;
        obs.entrant_count = booth.entrant_count;
    }

    // 2. rulebook
    doc rulebook_doc;
    obs.rulebook_visible = find_document(rulebook_doc, doc_type::rulebook,
                                         ui_section::inspection, screen);
    if (obs.rulebook_visible) {
        obs.rulebook_parsed_ok = parse_rulebook(obs.rules, rulebook_doc, ctx);
    }

    // 3. scan for documents
    auto detected = scan_documents(screen, ui_section::inspection);
    for (const auto& d : detected) {
        obs.docs_visible.push_back(d.type);

        // 4. extract fields per document type
        // (call your existing parse_* functions here and flatten into fields map)
    }

    return obs;
}