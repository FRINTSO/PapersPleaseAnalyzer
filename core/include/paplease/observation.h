#ifndef PAPLEASE_OBSERVATION_H
#define PAPLEASE_OBSERVATION_H
#include <map>
#include <optional>
#include <set>
#include <vector>

#include <paplease/date.h>
#include <paplease/rules.h>
#include <paplease/documents.h>
#include <paplease/inspector.h>

struct observation {
	bool booth_ok = false;
	date_t date;
	int entrant_count;
	std::optional<int> entrant_weight;

	// === RULEBOOK ===
	bool rulebook_visible = false;
	bool rulebook_ok = false;
	std::set<rule> rules;

	// === DOCUMENTS ===
	struct {
		std::vector<doc_type> visible;

		// Cross-doc identity fields (string comparison)
		std::map<doc_type, std::map<fact_field, std::string> > identity_fields;

		// Typed per-doc fields (checked against external values)
		std::map<doc_type, date_t> expirations;

		// structured data that doesn't fit the string map
		certificate_of_vaccination_data::vaccination_row vaccines[3];
		int vaccine_count = 0;

		std::optional<country> nationality;
	} entrant_docs;
};

struct observe_options {
	bool skip_rulebook = false;
};

observation observe_frame(const game_screen &screen, const resources_ctx &ctx,
			  const observe_options &opts = {});

#endif // PAPLEASE_OBSERVATION_H
