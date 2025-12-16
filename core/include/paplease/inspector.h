#ifndef PAPLEASE_INSPECTOR_H
#define PAPLEASE_INSPECTOR_H

#include <map>
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
	nationality,
};

struct fact_table {
	std::map<fact_field, std::map<doc_type, std::string> > cells{};

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

	// nationality (from passport)
	std::optional<country> nationality;

	// computed discrepancies
	std::vector<discrepancy> discrepancies;
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

#endif // PAPLEASE_INSPECTOR_H
