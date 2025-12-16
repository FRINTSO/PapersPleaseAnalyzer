#ifndef PAPLEASE_INSPECTOR_H
#define PAPLEASE_INSPECTOR_H

#include <map>
#include <optional>
#include <set>
#include <string_view>
#include <vector>

#include <paplease/documents.h>
#include <paplease/game_screen.h>
#include <paplease/resources.h>
#include <paplease/types.h>
#include <paplease/rules.h>

enum class fact_field {
	name,
	date_of_birth,
	passport_number,
	sex,
	issuing_city,
	nationality,
	district,
	purpose,
	duration,
	work_field,
	physical_desc,
	valid_date,
	height_cm,
	weight_kg,
	access_countries,
	end_date,

	expiration,
};

struct fact_table {
	std::map<fact_field, std::map<doc_type, std::string> > cells{};

	void put(fact_field f, doc_type src, const std::string &val)
	{
		cells[f][src] = val;
	}
};

// === DISCREPANCY: Cross-doc identity mismatch (unchanged) ===
enum class source {
	// Documents
	passport,
	entry_permit,
	work_pass,
	id_card,
	access_permit,
	vaccination_cert,
	asylum_grant,
	identity_supplement,

	// Booth
	booth_date,
	booth_scale,
};

enum class response {
	deny, // missing doc, expired, wrong papers
	interrogate, // ask questions (minor discrepancy)
	fingerprint, // verify identity (identity mismatch)
	detain, // criminal, smuggler, hidden items
};

enum class problem_kind {
	identity_mismatch,
	expired,
	weight_mismatch,
	height_mismatch,
	photo_mismatch,
	fingerprint_mismatch,
	invalid_seal,
};

struct claim {
	source who;
	std::string what;
};

struct problem {
	problem_kind kind;
	std::vector<claim> claims;
	response action;
};

struct case_file {
	// === IDENTITY (cross-doc, should match) ===
	fact_table facts;

	// === PER-DOC VALUES (checked individually against external source) ===
	std::map<doc_type, date_t> expirations; // check each against today

	// === OTHER ===
	// what documents did the entrant give me?
	std::set<doc_type> docs_seen;

	// Structured
	certificate_of_vaccination_data::vaccination_row vaccines[3];
	int vaccine_count = 0;
	std::optional<country> nationality;
	std::optional<int> booth_weight_kg; // will always have a value when there is an entrant

	// computed discrepancies
	std::vector<problem> problems;
	// ...
};

struct inspector {
	date_t current_date;
	std::set<rule> rules;
	bool rules_loaded = false;

	int current_entrant;
	case_file entrant_case;

	void (*inform_player)(std::string_view msg);
	std::string last_printed_rules;
	std::string last_advice;
};

void inspector_step(inspector &ins, const game_screen &screen,
		    const resources_ctx &ctx);
source doc_to_source(doc_type d);

#endif // PAPLEASE_INSPECTOR_H
