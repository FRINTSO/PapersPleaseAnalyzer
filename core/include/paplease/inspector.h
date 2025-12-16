#ifndef PAPLEASE_INSPECTOR_H
#define PAPLEASE_INSPECTOR_H

#include <optional>
#include <string_view>
#include <vector>

#include <paplease/documents.h>
#include <paplease/game_screen.h>
#include <paplease/resources.h>
#include <paplease/types.h>

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

struct entrant_docs {
	// Parsed data (valid when corresponding flag is true)
	std::optional<passport_data> passport;
	country nationality;
	std::optional<entry_permit_data> entry_permit;
	std::optional<entry_ticket_data> entry_ticket;
	std::optional<work_pass_data> work_pass;
	std::optional<access_permit_data> access_permit;
	std::optional<diplomatic_authorization_data> diplomatic_auth;
	std::optional<grant_of_asylum_data> grant_of_asylum;
	std::optional<identity_card_data> identity_card;
	std::optional<identity_supplement_data> identity_supplement;
	std::optional<certificate_of_vaccination_data> vaccination_cert;
};

struct inspector {
	// Days state
	date_t current_date;
	day_rules rules;
	bool has_rules;

	// Entrant state
	std::string current_entrant_count;
	entrant_docs entrant;
};

void process_game_frame(inspector &ins, const game_screen &screen,
			const resources_ctx &ctx);

#endif // PAPLEASE_INSPECTOR_H
