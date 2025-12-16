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

constexpr rule_def all_rules[] = {
	// === DOCUMENT REQUIREMENTS ===
	{ rule::entrant_needs_passport, condition::always, effect::require_doc,
	  doc_type::passport },

	{ rule::entrant_needs_polio_vaccine, condition::always,
	  effect::require_doc, doc_type::certificate_of_vaccination },

	{ rule::citizens_need_id_card, condition::is_citizen,
	  effect::require_doc, doc_type::identity_card },

	{ rule::foreigners_need_entry_permit, condition::is_foreigner,
	  effect::require_doc, doc_type::entry_permit },

	{ rule::foreigners_need_entry_ticket, condition::is_foreigner,
	  effect::require_doc, doc_type::entry_ticket },

	{ rule::foreigners_need_access_permit, condition::is_foreigner,
	  effect::require_doc, doc_type::access_permit },

	{ rule::foreigners_need_id_supplement, condition::is_foreigner,
	  effect::require_doc, doc_type::identity_supplement },

	{ rule::workers_need_work_pass, condition::is_worker,
	  effect::require_doc, doc_type::work_pass },

	{ rule::diplomats_need_authorization, condition::is_diplomat,
	  effect::require_doc, doc_type::diplomatic_authorization },

	{ rule::asylum_seekers_need_grant, condition::is_asylum_seeker,
	  effect::require_doc, doc_type::grant_of_asylum },

	// === VALIDITY ===
	{ rule::all_docs_must_be_current,
	  condition::always,
	  effect::check_expiration,
	  {} },

	// === PROHIBITIONS ===
	{ rule::arstotzkan_citizens_only,
	  condition::is_foreigner,
	  effect::prohibit_entry,
	  {} },

	{ rule::no_entry_from_impor,
	  condition::from_impor,
	  effect::prohibit_entry,
	  {} },

	{ rule::no_entry_from_united_federation,
	  condition::from_united_federation,
	  effect::prohibit_entry,
	  {} },

	{ rule::no_weapons_or_contraband,
	  condition::always,
	  effect::search_entrant, // or separate check
	  {} },

	// === SEARCH ===
	{ rule::kolechians_must_be_searched,
	  condition::is_kolechian,
	  effect::search_entrant,
	  {} },

	// === CONFISCATIONS ===
	{ rule::confiscate_arstotzkan_passports, condition::is_arstotzkan,
	  effect::confiscate_doc, doc_type::passport },

	{ rule::confiscate_altan_passports, condition::is_altan_resident,
	  effect::confiscate_doc, doc_type::passport },
};


#endif // _PAPLEASE_CORE_INSPECTOR_RULES_H
