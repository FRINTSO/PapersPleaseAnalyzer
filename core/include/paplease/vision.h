#ifndef PAPLEASE_VISION_H
#define PAPLEASE_VISION_H

#include <string>

#include <paplease/game_screen.h>

struct booth_info {
        std::string current_date;
	std::string entrant_count;
};

bool extract_booth_info(booth_info &out, const game_screen &screen);

std::vector<doc> scan_for_booth_documents(const game_screen& screen);
std::vector<doc_variant> scan_booth_for_doc_variants(const game_screen& screen);

std::vector<doc> scan_for_inspection_documents(const game_screen& screen);
std::vector<doc_variant> scan_inspection_for_doc_variants(const game_screen& screen);
#endif // PAPLEASE_VISION_H
