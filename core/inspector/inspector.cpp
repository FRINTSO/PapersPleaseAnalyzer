#include "paplease/resources.h"
#include <cstdio>
#include <iostream>
#include <string_view>

#include <magic_enum/magic_enum.hpp>

#include <paplease/compiler.h>
#include <paplease/documents.h>
#include <paplease/game_screen.h>
#include <paplease/inspector.h>
#include <paplease/vision.h>

#include "print.h"
#include "rulebook.h"

static constexpr int MAX_VACCINATIONS = 3;

// === UTILITIES ===

static void notify_player(std::string_view msg)
{
	printf("%.*s\n", (int)msg.size(), msg.data());
}

static bool date_expired(const date_t &doc_date, const date_t &current)
{
	if (doc_date.year < current.year)
		return true;
	if (doc_date.year > current.year)
		return false;
	if (doc_date.month < current.month)
		return true;
	if (doc_date.month > current.month)
		return false;
	return doc_date.day < current.day;
}

// === DOCUMENT PROCESSING ===

template <typename DataT, typename ParseFn, typename PrintFn>
static bool try_process(std::optional<DataT> &slot, const doc &document,
			const resources_ctx &ctx, ParseFn parse, PrintFn print)
{
	if (slot)
		return false; // Already have this doc

	DataT parsed;
	if (!parse(parsed, document, ctx))
		return false;

	slot = std::move(parsed);
	print(*slot);
	return true;
}

static void process_document(entrant_docs &docs, const doc &document,
			     const resources_ctx &ctx)
{
	switch (document.type) {
	case doc_type::passport:
		try_process(docs.passport, document, ctx, parse_passport,
			    print_passport);
		if (docs.passport)
			docs.nationality = document.issuing_country;
		break;
	case doc_type::entry_permit:
		try_process(docs.entry_permit, document, ctx,
			    parse_entry_permit, print_entry_permit);
		break;
	case doc_type::entry_ticket:
		try_process(docs.entry_ticket, document, ctx,
			    parse_entry_ticket, print_entry_ticket);
		break;
	case doc_type::work_pass:
		try_process(docs.work_pass, document, ctx, parse_work_pass,
			    print_work_pass);
		break;
	case doc_type::access_permit:
		try_process(docs.access_permit, document, ctx,
			    parse_access_permit, print_access_permit);
		break;
	case doc_type::diplomatic_authorization:
		try_process(docs.diplomatic_auth, document, ctx,
			    parse_diplomatic_authorization,
			    print_diplomatic_auth);
		break;
	case doc_type::grant_of_asylum:
		try_process(docs.grant_of_asylum, document, ctx,
			    parse_grant_of_asylum, print_grant_of_asylum);
		break;
	case doc_type::identity_card:
		try_process(docs.identity_card, document, ctx,
			    parse_identity_card, print_identity_card);
		break;
	case doc_type::identity_supplement:
		try_process(docs.identity_supplement, document, ctx,
			    parse_identity_supplement,
			    print_identity_supplement);
		break;
	case doc_type::certificate_of_vaccination:
		try_process(docs.vaccination_cert, document, ctx,
			    parse_certificate_of_vaccination,
			    print_vaccination_cert);
		break;
	default:
		break;
	}
}

// === DATA COLLECTION ===

template <typename T> struct sourced {
	T value;
	doc_type source;
};

static std::vector<sourced<std::string> > collect_names(const entrant_docs &e)
{
	std::vector<sourced<std::string> > out;
	if (e.passport)
		out.push_back({ e.passport->name, doc_type::passport });
	if (e.entry_permit)
		out.push_back({ e.entry_permit->name, doc_type::entry_permit });
	if (e.work_pass)
		out.push_back({ e.work_pass->name, doc_type::work_pass });
	if (e.access_permit)
		out.push_back(
			{ e.access_permit->name, doc_type::access_permit });
	if (e.diplomatic_auth)
		out.push_back({ e.diplomatic_auth->name,
				doc_type::diplomatic_authorization });
	if (e.grant_of_asylum)
		out.push_back(
			{ e.grant_of_asylum->name, doc_type::grant_of_asylum });
	if (e.identity_card)
		out.push_back(
			{ e.identity_card->name, doc_type::identity_card });
	if (e.vaccination_cert)
		out.push_back({ e.vaccination_cert->name,
				doc_type::certificate_of_vaccination });
	return out;
}

static std::vector<sourced<std::string> >
collect_passport_nums(const entrant_docs &e)
{
	std::vector<sourced<std::string> > out;
	if (e.passport)
		out.push_back(
			{ e.passport->passport_number, doc_type::passport });
	if (e.entry_permit)
		out.push_back({ e.entry_permit->passport_number,
				doc_type::entry_permit });
	if (e.access_permit)
		out.push_back({ e.access_permit->passport_number,
				doc_type::access_permit });
	if (e.diplomatic_auth)
		out.push_back({ e.diplomatic_auth->passport_number,
				doc_type::diplomatic_authorization });
	if (e.grant_of_asylum)
		out.push_back({ e.grant_of_asylum->passport_number,
				doc_type::grant_of_asylum });
	if (e.vaccination_cert)
		out.push_back({ e.vaccination_cert->passport_number,
				doc_type::certificate_of_vaccination });
	return out;
}

static std::vector<sourced<date_t> >
collect_dates_of_birth(const entrant_docs &e)
{
	std::vector<sourced<date_t> > out;
	if (e.passport)
		out.push_back(
			{ e.passport->date_of_birth, doc_type::passport });
	if (e.grant_of_asylum)
		out.push_back({ e.grant_of_asylum->date_of_birth,
				doc_type::grant_of_asylum });
	if (e.identity_card)
		out.push_back({ e.identity_card->date_of_birth,
				doc_type::identity_card });
	return out;
}
// === VALIDATION ===

static std::string_view get_purpose(const entrant_docs &e)
{
	if (e.entry_permit && !e.entry_permit->purpose.empty())
		return e.entry_permit->purpose;
	if (e.access_permit && !e.access_permit->purpose.empty())
		return e.access_permit->purpose;
	return "";
}

static bool has_polio_vaccine(const entrant_docs &e)
{
	if (!e.vaccination_cert)
		return false;
	for (int i = 0; i < MAX_VACCINATIONS; i++) {
		const std::string &name =
			e.vaccination_cert->vaccinations[i].name;
		if (name.find("POLIO") != std::string::npos ||
		    name.find("polio") != std::string::npos)
			return true;
	}
	return false;
}

static void check_missing_documents(const day_rules &rules,
				    const entrant_docs &docs)
{
	std::string_view purpose = get_purpose(docs);

	if (rules.require_passport && !docs.passport)
		notify_player("MISSING: Passport");

	if (rules.require_entry_permit && !docs.entry_permit &&
	    docs.nationality != country::arstotzka)
		notify_player("MISSING: Entry permit");

	if (rules.require_entry_ticket && !docs.entry_ticket &&
	    docs.nationality != country::arstotzka)
		notify_player("MISSING: Entry ticket");

	if (rules.require_work_pass && !docs.work_pass && purpose == "WORK")
		notify_player("MISSING: Work pass");

	if (rules.require_identity_card && !docs.identity_card &&
	    docs.nationality == country::arstotzka)
		notify_player("MISSING: ID card");

	if (rules.require_polio_vaccination && !has_polio_vaccine(docs))
		notify_player("MISSING: Polio vaccination");

	if (rules.require_diplomatic_authorization && !docs.diplomatic_auth &&
	    purpose == "DIPLOMAT")
		notify_player("MISSING: Diplomatic authorization");

	if (rules.require_asylum_grant && !docs.grant_of_asylum &&
	    purpose == "ASYLUM")
		notify_player("MISSING: Grant of asylum");
}

static void check_prohibitions(const day_rules &rules, const entrant_docs &e)
{
	if (rules.prohibit_impor && e.nationality == country::impor)
		notify_player("VIOLATION: Entry from Impor prohibited");

	if (rules.prohibit_united_federation &&
	    e.nationality == country::united_federation)
		notify_player(
			"VIOLATION: Entry from United Federation prohibited");
}

static void check_expirations(const day_rules &rules, const entrant_docs &e,
			      const date_t &current)
{
	if (!rules.all_docs_must_be_current)
		return;

	if (e.passport && date_expired(e.passport->expiration, current))
		notify_player("EXPIRED: Passport");

	if (e.entry_permit && date_expired(e.entry_permit->expiration, current))
		notify_player("EXPIRED: Entry permit");

	if (e.access_permit &&
	    date_expired(e.access_permit->expiration, current))
		notify_player("EXPIRED: Access permit");

	if (e.grant_of_asylum &&
	    date_expired(e.grant_of_asylum->expiration, current))
		notify_player("EXPIRED: Grant of asylum");
}

template <typename T>
static void check_discrepancies(const std::vector<sourced<T> > &values,
				const char *field)
{
	for (size_t i = 0; i < values.size(); i++) {
		for (size_t j = i + 1; j < values.size(); j++) {
			if (!values[i].value.empty() &&
			    !values[j].value.empty() &&
			    !(values[i].value == values[j].value)) {
				printf("DISCREPANCY: %s mismatch (%s vs %s)\n",
				       field,
				       magic_enum::enum_name(values[i].source)
					       .data(),
				       magic_enum::enum_name(values[j].source)
					       .data());
			}
		}
	}
}

static void check_discrepancies(const entrant_docs &e)
{
	check_discrepancies(collect_names(e), "Name");
	check_discrepancies(collect_passport_nums(e), "Passport number");
	check_discrepancies(collect_dates_of_birth(e), "Date of birth");
}

static void reset_inspector(inspector &ins)
{
	ins.current_date = {};
	ins.rules = {};
	ins.has_rules = {};

	// Entrant state
	ins.current_entrant_count = "";
	ins.entrant = {};
}

bool update_day(inspector &ins, const booth_info &booth)
{
	if (ins.current_date == booth.current_date)
		return false;
	ins.current_date = booth.current_date;
	ins.rules = {};
	ins.has_rules = false;
	ins.current_entrant_count.clear();
	ins.entrant = {};
	printf("=== NEW DAY: %u.%u.%u ===\n", (u8)booth.current_date.day,
	       (u8)booth.current_date.month, (u16)booth.current_date.year);
	return true;
}

bool update_entrant(inspector &ins, const booth_info &booth)
{
	if (ins.current_entrant_count == booth.entrant_count)
		return false;
	ins.current_entrant_count = booth.entrant_count;
	ins.entrant = {};
	printf("--- Entrant #%s ---\n", booth.entrant_count.c_str());
	return true;
}

bool ensure_rules_loaded(inspector &ins, const game_screen &screen,
			 const resources_ctx &ctx)
{
	if (ins.has_rules)
		return true;
	doc rulebook;
	if (!find_document(rulebook, doc_type::rulebook, ui_section::inspection,
			   screen))
		return false;
	day_rules parsed{};
	if (!parse_rulebook(parsed, rulebook, ctx)) {
		notify_player("Failed to parse rulebook");
		return false;
	}
	ins.rules = parsed;
	ins.has_rules = true;
	notify_player("Successfully parsed rulebook");
	return true;
}

void process_game_frame(inspector &ins, const game_screen &screen,
			const resources_ctx &ctx)
{
	booth_info booth;
	if (!extract_booth_info(booth, screen, ctx))
		return;

	update_day(ins, booth);
	update_entrant(ins, booth);

	if (!ensure_rules_loaded(ins, screen, ctx))
		return;

	/*
	// find documents and apply rules
	auto detected = scan_documents(screen, ui_section::inspection);
	for (const auto &d : detected) {
		process_document(ins.entrant, d, ctx);
	}

	check_missing_documents(ins.rules, ins.entrant);
	check_prohibitions(ins.rules, ins.entrant);
	check_expirations(ins.rules, ins.entrant, ins.current_date);
	check_discrepancies(ins.entrant);
	*/
}
