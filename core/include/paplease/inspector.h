#ifndef PAPLEASE_INSPECTOR_H
#define PAPLEASE_INSPECTOR_H

#include <optional>

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

struct doc_status {
	enum class state : u8 { seen_in_booth, seen_in_inspection, scanned };

	state current_state;
	std::optional<doc_data> data;
};

struct day_rules {
	// === REQUIREMENTS ===
	bool all_docs_must_be_current = false;
	bool require_passport = false;
	bool require_arstotzkan_passport = false;
	bool require_identity_card = false;
	bool require_entry_ticket = false;
	bool require_work_pass = false;
	bool require_diplomatic_authorization = false;
	bool require_identity_supplement = false;
	bool require_asylum_grant = false;
	bool require_polio_vaccination = false;
	bool require_access_permit = false;
	bool require_entry_permit = false;
	bool require_search_kolechians = false;

	// === PROHIBITIONS ===
	bool prohibit_impor = false;
	bool prohibit_united_federation = false;
	bool prohibit_weapons_and_contraband = false;

	// === CONFISCATIONS ===
	bool confiscate_altan_passports = false;
	bool confiscate_arstotzkan_passports = false;
};

// Accumulated facts about the current entrant
struct entrant_info {
	// === IDENTITY (from multiple docs) ===
	std::string name;              // from passport, permits, etc
	std::string passport_number;   // cross-reference key
	date_t date_of_birth;
	bool is_male;
	country nationality;           // from passport variant

	// === PHYSICAL (for discrepancy checks) ===
	u16 height_cm;
	u16 weight_kg;
	std::string physical_desc;
	std::string district;          // from ID card

	// === DOCUMENTS PRESENT ===
	bool has_passport;
	bool has_entry_permit;
	bool has_entry_ticket;
	bool has_work_pass;
	bool has_access_permit;
	bool has_diplomatic_auth;
	bool has_grant_of_asylum;
	bool has_identity_card;
	bool has_identity_supplement;
	bool has_vaccination_cert;

	// === KEY DATES (for expiration checks) ===
	date_t passport_expiration;
	date_t permit_expiration;      // entry_permit
	date_t ticket_valid_date;      // entry_ticket
	date_t work_pass_end;
	date_t access_permit_exp;
	date_t asylum_expiration;
	date_t id_supplement_exp;

	// === PURPOSE/WORK ===
	std::string purpose;           // from permits
	std::string duration;
	std::string work_field;

	// === DIPLOMATIC ===
	std::string diplomatic_access_countries;

	// === VACCINATION ===
	bool has_polio_vaccine;

	// === FOR DISCREPANCY DETECTION ===
	// Store names/numbers from each doc to cross-check
	std::string passport_name;
	std::string permit_name;
	std::string id_card_name;
	std::string passport_num_from_permit;
};

struct inspector {
	// Days state
	std::string current_date;
	day_rules rules;
	bool has_rules;

	// Entrant state
	std::string current_entrant_count;
	entrant_info entrant;

	// Funcs
	void process_game_frame(const game_screen &screen);
};

#endif // PAPLEASE_INSPECTOR_H
