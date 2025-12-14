#include <cstdio>

#include <iostream>

#include <paplease/compiler.h>
#include <paplease/documents.h>
#include <paplease/game_screen.h>
#include <paplease/inspector.h>
#include <paplease/vision.h>

#include "rulebook.h"

static void reset_inspector(inspector &ins)
{
	ins.current_date = "";
	ins.rules = {};
	ins.has_rules = {};

	// Entrant state
	ins.current_entrant_count = "";
	ins.entrant = {};
}

static void notify_player(std::string_view msg)
{
	std::cout << msg << '\n';
}
static bool date_is_set(const date_t &d)
{
	return d.year != 0;
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
static void process_passport(entrant_info &e, const doc &document)
{
	if (e.has_passport)
		return;  // already processed

	passport_data data;
	if (!parse_passport(data, document))
		return;

	e.has_passport = true;
	e.nationality = country_of(document.variant);
	e.passport_name = data.name;
	e.passport_number = data.passport_number;
	e.date_of_birth = data.date_of_birth;
	e.passport_expiration = data.expiration;
	e.is_male = data.is_male;

	if (e.name.empty())
		e.name = data.name;

	printf("  [PASSPORT]\n");
	printf("    name:       %s\n", data.name.c_str());
	printf("    number:     %s\n", data.passport_number.c_str());
	printf("    city:       %s\n", data.issuing_city.c_str());
	printf("    dob:        %02d.%02d.%d\n", data.date_of_birth.day, data.date_of_birth.month, data.date_of_birth.year);
	printf("    expiration: %02d.%02d.%d\n", data.expiration.day, data.expiration.month, data.expiration.year);
	printf("    sex:        %s\n", data.is_male ? "M" : "F");
}

static void process_entry_permit(entrant_info &e, const doc &document)
{
	if (e.has_entry_permit)
		return;

	entry_permit_data data;
	if (!parse_entry_permit(data, document))
		return;

	e.has_entry_permit = true;
	e.permit_name = data.name;
	e.passport_num_from_permit = data.passport_number;
	e.purpose = data.purpose;
	e.duration = data.duration;
	e.permit_expiration = data.expiration;

	printf("  [ENTRY PERMIT]\n");
	printf("    name:       %s\n", data.name.c_str());
	printf("    passport:   %s\n", data.passport_number.c_str());
	printf("    purpose:    %s\n", data.purpose.c_str());
	printf("    duration:   %s\n", data.duration.c_str());
	printf("    expiration: %02d.%02d.%d\n", data.expiration.day, data.expiration.month, data.expiration.year);
}

static void process_entry_ticket(entrant_info &e, const doc &document)
{
	if (e.has_entry_ticket)
		return;

	entry_ticket_data data;
	if (!parse_entry_ticket(data, document))
		return;

	e.has_entry_ticket = true;
	e.ticket_valid_date = data.valid_date;

	printf("  [ENTRY TICKET]\n");
	printf("    valid_date: %02d.%02d.%d\n", data.valid_date.day, data.valid_date.month, data.valid_date.year);
}

static void process_work_pass(entrant_info &e, const doc &document)
{
	if (e.has_work_pass)
		return;

	work_pass_data data;
	if (!parse_work_pass(data, document))
		return;

	e.has_work_pass = true;
	e.work_field = data.work_field;
	e.work_pass_end = data.end_date;

	printf("  [WORK PASS]\n");
	printf("    name:     %s\n", data.name.c_str());
	printf("    field:    %s\n", data.work_field.c_str());
	printf("    end_date: %02d.%02d.%d\n", data.end_date.day, data.end_date.month, data.end_date.year);
}

static void process_access_permit(entrant_info &e, const doc &document)
{
	if (e.has_access_permit)
		return;

	access_permit_data data;
	if (!parse_access_permit(data, document))
		return;

	e.has_access_permit = true;
	e.access_permit_exp = data.expiration;
	e.height_cm = data.height_cm;
	e.weight_kg = data.weight_kg;
	e.physical_desc = data.physical_desc;

	printf("  [ACCESS PERMIT]\n");
	printf("    name:       %s\n", data.name.c_str());
	printf("    passport:   %s\n", data.passport_number.c_str());
	printf("    country:    %s\n", data.issuing_country.c_str());
	printf("    purpose:    %s\n", data.purpose.c_str());
	printf("    duration:   %s\n", data.duration.c_str());
	printf("    height:     %dcm\n", data.height_cm);
	printf("    weight:     %dkg\n", data.weight_kg);
	printf("    physical:   %s\n", data.physical_desc.c_str());
	printf("    expiration: %02d.%02d.%d\n", data.expiration.day, data.expiration.month, data.expiration.year);
}

static void process_diplomatic_auth(entrant_info &e, const doc &document)
{
	if (e.has_diplomatic_auth)
		return;

	diplomatic_authorization_data data;
	if (!parse_diplomatic_authorization(data, document))
		return;

	e.has_diplomatic_auth = true;
	e.diplomatic_access_countries = data.access_countries;

	printf("  [DIPLOMATIC AUTH]\n");
	printf("    name:     %s\n", data.name.c_str());
	printf("    passport: %s\n", data.passport_number.c_str());
	printf("    from:     %s\n", data.issuing_country.c_str());
	printf("    access:   %s\n", data.access_countries.c_str());
}

static void process_grant_of_asylum(entrant_info &e, const doc &document)
{
	if (e.has_grant_of_asylum)
		return;

	grant_of_asylum_data data;
	if (!parse_grant_of_asylum(data, document))
		return;

	e.has_grant_of_asylum = true;
	e.asylum_expiration = data.expiration;

	printf("  [GRANT OF ASYLUM]\n");
	printf("    name:       %s\n", data.name.c_str());
	printf("    passport:   %s\n", data.passport_number.c_str());
	printf("    from:       %s\n", data.issuing_country.c_str());
	printf("    dob:        %02d.%02d.%d\n", data.date_of_birth.day, data.date_of_birth.month, data.date_of_birth.year);
	printf("    height:     %dcm\n", data.height_cm);
	printf("    weight:     %dkg\n", data.weight_kg);
	printf("    expiration: %02d.%02d.%d\n", data.expiration.day, data.expiration.month, data.expiration.year);
}

static void process_identity_card(entrant_info &e, const doc &document)
{
	if (e.has_identity_card)
		return;

	identity_card_data data;
	if (!parse_identity_card(data, document))
		return;

	e.has_identity_card = true;
	e.id_card_name = data.name;
	e.district = data.district;

	if (e.height_cm == 0)
		e.height_cm = data.height_cm;
	if (e.weight_kg == 0)
		e.weight_kg = data.weight_kg;

	printf("  [ID CARD]\n");
	printf("    name:     %s\n", data.name.c_str());
	printf("    district: %s\n", data.district.c_str());
	printf("    dob:      %02d.%02d.%d\n", data.date_of_birth.day, data.date_of_birth.month, data.date_of_birth.year);
	printf("    height:   %dcm\n", data.height_cm);
	printf("    weight:   %dkg\n", data.weight_kg);
}

static void process_identity_supplement(entrant_info &e, const doc &document)
{
	if (e.has_identity_supplement)
		return;

	identity_supplement_data data;
	if (!parse_identity_supplement(data, document))
		return;

	e.has_identity_supplement = true;
	e.id_supplement_exp = data.expiration;
	e.physical_desc = data.physical_desc;

	printf("  [ID SUPPLEMENT]\n");
	printf("    height:     %dcm\n", data.height_cm);
	printf("    weight:     %dkg\n", data.weight_kg);
	printf("    physical:   %s\n", data.physical_desc.c_str());
	printf("    expiration: %02d.%02d.%d\n", data.expiration.day, data.expiration.month, data.expiration.year);
}

static void process_vaccination_cert(entrant_info &e, const doc &document)
{
	if (e.has_vaccination_cert)
		return;

	certificate_of_vaccination_data data;
	if (!parse_certificate_of_vaccination(data, document))
		return;

	e.has_vaccination_cert = true;

	for (int i = 0; i < 3; i++) {
		if (data.vaccinations[i].name.find("POLIO") != std::string::npos ||
		    data.vaccinations[i].name.find("polio") != std::string::npos) {
			e.has_polio_vaccine = true;
		}
	}

	printf("  [VACCINATION CERT]\n");
	printf("    name:     %s\n", data.name.c_str());
	printf("    passport: %s\n", data.passport_number.c_str());
	for (int i = 0; i < 3; i++) {
		if (!data.vaccinations[i].name.empty()) {
			printf("    vaccine%d: %s (%02d.%02d.%d)\n", i + 1,
			       data.vaccinations[i].name.c_str(),
			       data.vaccinations[i].expiration_date.day,
			       data.vaccinations[i].expiration_date.month,
			       data.vaccinations[i].expiration_date.year);
		}
	}
	printf("    polio:    %s\n", e.has_polio_vaccine ? "YES" : "NO");
}

static void process_document(entrant_info &e, const doc &document)
{
	switch (type_of(document.variant)) {
	case doc_type::passport:
		process_passport(e, document);
		break;
	case doc_type::entry_permit:
		process_entry_permit(e, document);
		break;
	case doc_type::entry_ticket:
		process_entry_ticket(e, document);
		break;
	case doc_type::work_pass:
		process_work_pass(e, document);
		break;
	case doc_type::access_permit:
		process_access_permit(e, document);
		break;
	case doc_type::diplomatic_authorization:
		process_diplomatic_auth(e, document);
		break;
	case doc_type::grant_of_asylum:
		process_grant_of_asylum(e, document);
		break;
	case doc_type::identity_card:
		process_identity_card(e, document);
		break;
	case doc_type::identity_supplement:
		process_identity_supplement(e, document);
		break;
	case doc_type::certificate_of_vaccination:
		process_vaccination_cert(e, document);
		break;
	default:
	}
}
void check_missing_documents(const day_rules &rules, const entrant_info &e)
{
	if (rules.require_passport && !e.has_passport)
		notify_player("MISSING: Passport");

	if (rules.require_entry_permit && !e.has_entry_permit &&
	    e.nationality != country::arstotzka)
		notify_player("MISSING: Entry permit");

	if (rules.require_entry_ticket && !e.has_entry_ticket &&
	    e.nationality != country::arstotzka)
		notify_player("MISSING: Entry ticket");

	if (rules.require_work_pass && !e.has_work_pass && e.purpose == "WORK")
		notify_player("MISSING: Work pass");

	if (rules.require_identity_card && !e.has_identity_card &&
	    e.nationality == country::arstotzka)
		notify_player("MISSING: ID card");

	if (rules.require_polio_vaccination && !e.has_polio_vaccine)
		notify_player("MISSING: Polio vaccination");

	if (rules.require_diplomatic_authorization && !e.has_diplomatic_auth &&
	    e.purpose == "DIPLOMAT")
		notify_player("MISSING: Diplomatic authorization");

	if (rules.require_asylum_grant && !e.has_grant_of_asylum &&
	    e.purpose == "ASYLUM")
		notify_player("MISSING: Grant of asylum");
}

void check_prohibitions(const day_rules &rules, const entrant_info &e)
{
	if (rules.prohibit_impor && e.nationality == country::impor)
		notify_player("VIOLATION: Entry from Impor prohibited");

	if (rules.prohibit_united_federation &&
	    e.nationality == country::united_federation)
		notify_player(
			"VIOLATION: Entry from United Federation prohibited");
}

void check_expirations(const day_rules &rules, const entrant_info &e,
		       const date_t &current)
{
	if (!rules.all_docs_must_be_current)
		return;

	if (e.has_passport && date_expired(e.passport_expiration, current))
		notify_player("EXPIRED: Passport");

	if (e.has_entry_permit && date_expired(e.permit_expiration, current))
		notify_player("EXPIRED: Entry permit");

	if (e.has_access_permit && date_expired(e.access_permit_exp, current))
		notify_player("EXPIRED: Access permit");

	if (e.has_grant_of_asylum && date_expired(e.asylum_expiration, current))
		notify_player("EXPIRED: Grant of asylum");
}

void check_discrepancies(const entrant_info &e)
{
	// Name mismatches
	if (!e.passport_name.empty() && !e.permit_name.empty() &&
	    e.passport_name != e.permit_name)
		notify_player(
			"DISCREPANCY: Name mismatch (passport vs permit)");

	if (!e.passport_name.empty() && !e.id_card_name.empty() &&
	    e.passport_name != e.id_card_name)
		notify_player(
			"DISCREPANCY: Name mismatch (passport vs ID card)");

	// Passport number mismatch
	if (!e.passport_number.empty() && !e.passport_num_from_permit.empty() &&
	    e.passport_number != e.passport_num_from_permit)
		notify_player("DISCREPANCY: Passport number mismatch");
}
void inspector::process_game_frame(const game_screen &screen)
{
	booth_info booth;
	if (!extract_booth_info(booth, screen))
		return;

	if (this->current_date != booth.current_date) {
		// reset inspector
		reset_inspector(*this);
		this->current_date = booth.current_date;
		printf("=== NEW DAY: %s ===\n", booth.current_date.c_str());
	}

	if (this->current_entrant_count != booth.entrant_count) {
		this->current_entrant_count = booth.entrant_count;
		this->entrant = {};
		printf("--- Entrant #%s ---\n", booth.entrant_count.c_str());
	}

	// find and scan rulebook
	doc rulebook;
	if (!this->has_rules && find_document(rulebook, doc_type::rulebook,
					      ui_section::inspection, screen)) {
		day_rules rules;
		if (!parse_rulebook(rules, rulebook)) {
			return;
		}
		notify_player("Successfully parsed rulebook");
		this->rules = rules;
		this->has_rules = true;
	}

	if (!this->has_rules)
		return;

	// find documents and apply rules
	std::vector<doc> documents =
		find_all_documents(screen, ui_section::inspection);
	for (const doc &document : documents) {
		process_document(this->entrant, document);
	}

	check_missing_documents(this->rules, this->entrant);
	check_prohibitions(this->rules, this->entrant);
	// check_expirations(this->rules, this->entrant, current_date);
	check_discrepancies(this->entrant);
}
