#pragma once
#include <string>
#include <map>
#include <optional>
#include <paplease/documents.h>
#include <paplease/game_screen.h>
#include <paplease/resources.h>

struct observation {
    // booth signals (lifecycle)
    date_t date;
    std::string entrant_count;
    bool booth_readable = false;

    // rulebook
    bool rulebook_visible = false;
    bool rulebook_parsed_ok = false;
    day_rules rules;  // only valid if rulebook_parsed_ok

    // documents detected this frame
    std::vector<doc_type> docs_visible;

    // extracted fields per doc (only populated if doc was parseable)
    // fields[doc_type][field_name] = value
    std::map<doc_type, std::map<std::string, std::string>> fields;
};

observation observe_frame(const game_screen& screen, const resources_ctx& ctx);