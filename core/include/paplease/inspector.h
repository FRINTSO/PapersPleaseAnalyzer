#ifndef PAPLEASE_INSPECTOR_H
#define PAPLEASE_INSPECTOR_H

#include <map>
#include <optional>
#include <set>
#include <string_view>
#include <vector>

#include <paplease/date.h>
#include <paplease/documents.h>
#include <paplease/game_screen.h>
#include <paplease/types.h>

// ── Inspector types ──────────────────────────────────────────────

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
	certificate_of_vaccination_data::vaccination_row vaccines[MAX_VACCINATIONS];
	int vaccine_count = 0;
	std::optional<country> nationality;
	std::optional<int> booth_weight_kg; // will always have a value when there is an entrant

	// computed discrepancies
	std::vector<problem> problems;
	// ...
};

enum class rule {
	// === DOCUMENT REQUIREMENTS ===
	entrant_needs_passport,
	entrant_needs_polio_vaccine,
	citizens_need_id_card,
	foreigners_need_entry_permit,
	foreigners_need_entry_ticket,
	foreigners_need_access_permit,
	foreigners_need_id_supplement,
	workers_need_work_pass,
	diplomats_need_authorization,
	asylum_seekers_need_grant,

	// === VALIDITY ===
	all_docs_must_be_current,

	// === PROHIBITIONS ===
	arstotzkan_citizens_only,
	no_entry_from_impor,
	no_entry_from_united_federation,
	no_weapons_or_contraband,

	// === SEARCH ===
	kolechians_must_be_searched,

	// === CONFISCATIONS ===
	confiscate_arstotzkan_passports,
	confiscate_altan_passports,
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

// ── Observation ──────────────────────────────────────────────────

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

// ── Public API ───────────────────────────────────────────────────

void inspector_step(inspector &ins, const game_screen &screen);

observation observe_frame(const game_screen &screen,
			  const observe_options &opts = {});

source doc_to_source(doc_type d);

#endif // PAPLEASE_INSPECTOR_H
