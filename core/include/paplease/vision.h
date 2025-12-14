#ifndef PAPLEASE_VISION_H
#define PAPLEASE_VISION_H

#include <string>

#include <paplease/game_screen.h>

struct booth_info {
	std::string current_date;
	std::string entrant_count;
};

bool extract_booth_info(booth_info &out, const game_screen &screen);

bool find_document(doc &out, doc_type type, ui_section section, const game_screen& screen);

#endif // PAPLEASE_VISION_H
