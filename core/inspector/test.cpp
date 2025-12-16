#if 0
#include <map>
#include <set>
#include <string>
#include <vector>

#include <paplease/date.h>
#include <paplease/vision.h>
#include <paplease/documents.h>
#include <paplease/game_screen.h>
#include <paplease/resources.h>

#include "rules.h"

enum class fact_field {
	name,
	date_of_birth,
	passport_number,
	sex,
	issuing_city,
	issuing_country,
	district,
	purpose,
	duration,
	work_field,
	physical_desc,
	valid_date,
	end_date,
	expiration,
	height_cm,
	weight_kg,
	access_countries,
};

struct observation {
	bool booth_ok = false;
	date_t date;
	std::string entrant_count;

	// === RULEBOOK ===
	bool rulebook_visible = false;
	bool rulebook_ok = false;
	std::set<rule> rules;

	// === DOCUMENTS ===
	struct {
		std::vector<doc_type> visible;
		std::map<doc_type, std::map<fact_field, std::string> > fields;

		// structured data that doesn't fit the string map
		certificate_of_vaccination_data::vaccination_row vaccines[3];
		int vaccine_count = 0;
	} entrant_docs;
};

struct observe_options {
	bool skip_rulebook = false;
};

observation observe_frame(const game_screen &screen, const resources_ctx &ctx,
			  const observe_options &opts = {})
{
	observation obs;

	// === BOOTH ===
	booth_info booth;
	obs.booth_ok = extract_booth_info(booth, screen, ctx);
	if (obs.booth_ok) {
		obs.date = booth.current_date;
		obs.entrant_count = booth.entrant_count;
	}

	// === RULEBOOK ===
	if (!opts.skip_rulebook) {
		doc rulebook;
		obs.rulebook_visible =
			find_document(rulebook, doc_type::rulebook,
				      ui_section::inspection, screen);
		if (obs.rulebook_visible) {
			obs.rulebook_ok = parse_rules(obs.rules, rulebook, ctx);
		}
	}

	// === ENTRANT DOCUMENTS ===
	auto detected = scan_documents(screen, ui_section::inspection);

	for (const auto &d : detected) {
		// skip non-entrant docs
		if (d.type == doc_type::rulebook ||
		    d.type == doc_type::bulletin ||
		    d.type == doc_type::transcript)
			continue;

		obs.entrant_docs.visible.push_back(d.type);
		auto &f = obs.entrant_docs.fields[d.type];

		switch (d.type) {
		case doc_type::passport: {
			passport_data data;
			if (parse_passport(data, d, ctx)) {
				f[fact_field::name] = data.name;
				f[fact_field::passport_number] =
					data.passport_number;
				f[fact_field::date_of_birth] =
					format_date(data.date_of_birth);
				f[fact_field::expiration] =
					format_date(data.expiration);
			}
			break;
		}

		case doc_type::entry_permit: {
			entry_permit_data data;
			if (parse_entry_permit(data, d, ctx)) {
				f[fact_field::name] = data.name;
				f[fact_field::passport_number] =
					data.passport_number;
				f[fact_field::purpose] = data.purpose;
				f[fact_field::duration] = data.duration;
				f[fact_field::expiration] =
					format_date(data.expiration);
			}
			break;
		}

		case doc_type::entry_ticket: {
			entry_ticket_data data;
			if (parse_entry_ticket(data, d, ctx)) {
				f[fact_field::valid_date] =
					format_date(data.valid_date);
			}
			break;
		}

		case doc_type::work_pass: {
			work_pass_data data;
			if (parse_work_pass(data, d, ctx)) {
				f[fact_field::name] = data.name;
				f[fact_field::work_field] = data.work_field;
				f[fact_field::end_date] =
					format_date(data.end_date);
			}
			break;
		}

		case doc_type::access_permit: {
			access_permit_data data;
			if (parse_access_permit(data, d, ctx)) {
				f[fact_field::name] = data.name;
				f[fact_field::passport_number] =
					data.passport_number;
				f[fact_field::purpose] = data.purpose;
				f[fact_field::duration] = data.duration;
				f[fact_field::height_cm] =
					std::to_string(data.height_cm);
				f[fact_field::weight_kg] =
					std::to_string(data.weight_kg);
				f[fact_field::expiration] =
					format_date(data.expiration);
			}
			break;
		}

		case doc_type::identity_card: {
			identity_card_data data;
			if (parse_identity_card(data, d, ctx)) {
				f[fact_field::name] = data.name;
				f[fact_field::district] = data.district;
				f[fact_field::date_of_birth] =
					format_date(data.date_of_birth);
				f[fact_field::height_cm] =
					std::to_string(data.height_cm);
				f[fact_field::weight_kg] =
					std::to_string(data.weight_kg);
			}
			break;
		}

		case doc_type::grant_of_asylum: {
			grant_of_asylum_data data;
			if (parse_grant_of_asylum(data, d, ctx)) {
				f[fact_field::name] = data.name;
				f[fact_field::passport_number] =
					data.passport_number;
				f[fact_field::date_of_birth] =
					format_date(data.date_of_birth);
				f[fact_field::height_cm] =
					std::to_string(data.height_cm);
				f[fact_field::weight_kg] =
					std::to_string(data.weight_kg);
				f[fact_field::expiration] =
					format_date(data.expiration);
			}
			break;
		}

		case doc_type::diplomatic_authorization: {
			diplomatic_authorization_data data;
			if (parse_diplomatic_authorization(data, d, ctx)) {
				f[fact_field::name] = data.name;
				f[fact_field::passport_number] =
					data.passport_number;
				f[fact_field::access_countries] =
					data.access_countries;
			}
			break;
		}

		case doc_type::identity_supplement: {
			identity_supplement_data data;
			if (parse_identity_supplement(data, d, ctx)) {
				f[fact_field::height_cm] =
					std::to_string(data.height_cm);
				f[fact_field::weight_kg] =
					std::to_string(data.weight_kg);
				f[fact_field::physical_desc] =
					data.physical_desc;
				f[fact_field::expiration] =
					format_date(data.expiration);
			}
			break;
		}

		case doc_type::certificate_of_vaccination: {
			certificate_of_vaccination_data data;
			if (parse_certificate_of_vaccination(data, d, ctx)) {
				f[fact_field::name] = data.name;
				f[fact_field::passport_number] =
					data.passport_number;
				// store vaccines if needed
				constexpr size_t MAX_VACCINES =
					sizeof(data.vaccinations) /
					sizeof(data.vaccinations[0]);
				for (int i = 0; i < MAX_VACCINES; i++) {
					obs.entrant_docs.vaccines[i] =
						data.vaccinations[i];
				}
			}
			break;
		}

		default:
			break;
		}
	}

	return obs;
}

struct fact_table {
	std::map<fact_field, std::map<doc_type, std::string> > cells;

	void put(fact_field f, doc_type src, const std::string &val)
	{
		cells[f][src] = val;
	}

	std::vector<std::string> distinct_values(fact_field f) const
	{
		std::vector<std::string> out;
		auto it = cells.find(f);
		if (it == cells.end())
			return out;
		for (auto &[src, val] : it->second) {
			if (std::find(out.begin(), out.end(), val) == out.end())
				out.push_back(val);
		}
		return out;
	}
};

struct discrepancy {
	fact_field field; // e.g. fact_field::name
	std::vector<std::pair<doc_type, std::string> >
		values; // conflicting values + sources
};

struct case_file {
	// what documents did the entrant give me?
	std::set<doc_type> docs_seen;

	fact_table facts;

	// vaccines (from vaccination cert)
	certificate_of_vaccination_data::vaccination_row vaccines[3];
	int vaccine_count = 0;

	// computed discrepancies
	std::vector<discrepancy> discrepancies;
};

std::vector<discrepancy> compute_discrepancies(const fact_table &facts)
{
	std::vector<discrepancy> out;

	for (auto &[field, sources] : facts.cells) {
		// collect distinct values
		std::set<std::string> distinct;
		for (auto &[src, val] : sources) {
			distinct.insert(val);
		}

		// if more than one distinct value -> discrepancy
		if (distinct.size() > 1) {
			discrepancy d;
			d.field = field;
			for (auto &[src, val] : sources) {
				d.values.push_back({ src, val });
			}
			out.push_back(d);
		}
	}

	return out;
}

void update_case(case_file &cf, const observation &obs)
{
	for (doc_type dt : obs.entrant_docs.visible) {
		cf.docs_seen.insert(dt);
	}

	// 2. Merge fields into facts table
	for (auto &[doc, field_map] : obs.entrant_docs.fields) {
		for (auto &[field, value] : field_map) {
			if (!value.empty()) {
				cf.facts.put(field, doc, value);
			}
		}
	}

	// 4. Copy vaccines
	if (obs.entrant_docs.vaccine_count > 0) {
		cf.vaccine_count = obs.entrant_docs.vaccine_count;
		for (int i = 0; i < cf.vaccine_count; i++) {
			cf.vaccines[i] = obs.entrant_docs.vaccines[i];
		}
	}

	// 6. Recompute discrepancies
	cf.discrepancies = compute_discrepancies(cf.facts);
}

struct inspector_v2 {
	date_t current_date;
	std::set<rule> rules;
	bool rules_loaded = false;

	int current_entrant;
	case_file entrant_case;
};

struct advice {};

advice decide(const std::set<rule> &rules, const case_file &cf)
{
	return {};
}

void inspector_step(inspector_v2 &ins, const game_screen &screen,
		    const resources_ctx &ctx)
{
	auto obs = observe_frame(screen, ctx);

	update_case(ins.entrant_case, obs);

	advice adv = decide(ins.rules, ins.entrant_case);
}
#endif
