#ifndef _PAPLEASE_CORE_INSPECTOR_RULES_H
#define _PAPLEASE_CORE_INSPECTOR_RULES_H
#include <set>

#include <paplease/documents.h>

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

enum class condition {
	always,
	is_citizen,
	is_foreigner,
	is_worker,
	is_diplomat,
	is_asylum_seeker,
	is_kolechian,
	from_impor,
	from_united_federation,
	is_arstotzkan, // add this (used by confiscate_arstotzkan_passports)
	is_altan_resident, // add this (used by confiscate_altan_passports)
};

enum class effect {
	require_doc,
	prohibit_entry,
	confiscate_doc,
	search_entrant,
	check_expiration,
};

struct rule_def {
	rule id;
	condition when;
	effect what;
	doc_type target_doc; // if effect == require_doc or confiscate_doc
};


bool parse_rules(std::set<rule> &out, const doc &rulebook,
		    const resources_ctx &ctx);

#endif // _PAPLEASE_CORE_INSPECTOR_RULES_H
