#ifndef PAPLEASE_INSPECTOR_H
#define PAPLEASE_INSPECTOR_H

#include <optional>
#include <unordered_map>
#include <unordered_set>

#include <paplease/documents.h>
#include <paplease/game_screen.h>
#include <paplease/types.h>

struct entrant_class {
	// Nationality axis
	bool is_citizen; // true = Arstotzkan, false = foreigner
	bool nationality_known; // do we know yet?

	// Purpose axis
	enum class purpose { unknown, visitor, worker, diplomat, asylum };
	purpose visit_purpose;

	// Criminal axis
	bool is_wanted; // matches criminal bulletin
};

struct day_rules {
	// Universal rules
	bool require_passport; // always true basically
	bool all_docs_must_be_current; // expiry checks

	// Conditional rules
	bool citizens_need_id; // applies if is_citizen
	bool foreigners_need_permit; // applies if !is_citizen
	bool workers_need_work_pass; // applies if purpose == worker
	bool diplomats_need_auth; // applies if purpose == diplomat
	bool asylum_needs_grant; // applies if purpose == asylum

	// Bans
	std::unordered_set<country> banned_countries;
};

struct doc_status {
	enum class state : u8 { seen_in_booth, seen_in_inspection, scanned };

	state current_state;
	std::optional<doc_data> data;
};

struct inspector {
	// Days state
	std::string current_date;
	day_rules rules;
	bool has_rules;

	// Entrant state
	std::string current_entrant_count;
	entrant_class classification;

	std::unordered_map<doc_variant, doc_status> documents;

	// Funcs
	void init();
	void process_game_frame(const game_screen &screen);
};

#endif // PAPLEASE_INSPECTOR_H
