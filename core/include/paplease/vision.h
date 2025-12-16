#ifndef PAPLEASE_VISION_H
#define PAPLEASE_VISION_H

#include <string>

#include <paplease/game_screen.h>
#include <paplease/resources.h>
#include <vector>

struct booth_info {
	date_t current_date;
	std::string entrant_count;
	std::optional<int> entrant_weight;
};

bool extract_booth_info(booth_info &out, const game_screen &screen,
			const resources_ctx &ctx);

bool find_document(doc &out, doc_type type, ui_section section,
		   const game_screen &screen);
std::vector<doc> scan_documents(const game_screen &screen, ui_section section);

#endif // PAPLEASE_VISION_H
