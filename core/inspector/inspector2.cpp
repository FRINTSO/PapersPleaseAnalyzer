#include "paplease/date.h"
#include <optional>
#include <string>

#include <magic_enum/magic_enum.hpp>

#include <paplease/compiler.h>
#include <paplease/documents.h>
#include <paplease/rules.h>
#include <paplease/inspector.h>
#include <paplease/vision.h>

#include "observation.h"
#include <vector>

// Normalize name for comparison
std::string normalize_name(const std::string &raw)
{
	// "Doe, John" → "john doe"
	// "JOHN DOE" → "john doe"

	std::string s = raw;

	// Lowercase
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);

	// Handle "Last, First" format
	auto comma = s.find(',');
	if (comma != std::string::npos) {
		std::string last = s.substr(0, comma);
		std::string first = s.substr(comma + 1);
		// Trim whitespace
		first.erase(0, first.find_first_not_of(' '));
		first.erase(first.find_last_not_of(' ') + 1);
		last.erase(0, last.find_first_not_of(' '));
		last.erase(last.find_last_not_of(' ') + 1);
		s = first + " " + last;
	}

	return s;
}

static date_t normalize_date(const std::string &raw)
{
	date_t date{};
	const char *p = raw.c_str();

	// Skip leading whitespace
	while (*p && std::isspace(*p))
		p++;

	// Day (2 digits)
	if (!std::isdigit(p[0]) || !std::isdigit(p[1]))
		return date;
	date.day = (p[0] - '0') * 10 + (p[1] - '0');
	p += 2;

	// Separator (dot)
	if (*p == '.')
		p++;

	// Month (2 digits)
	if (!std::isdigit(p[0]) || !std::isdigit(p[1]))
		return date;
	date.month = (p[0] - '0') * 10 + (p[1] - '0');
	p += 2;

	// Separator (dot)
	if (*p == '.')
		p++;

	// Year (2 or 4 digits)
	int year = 0;
	int digits = 0;
	while (std::isdigit(*p) && digits < 4) {
		year = year * 10 + (*p - '0');
		p++;
		digits++;
	}

	// 2-digit year → 19xx
	if (digits == 2) {
		year += 1900;
	}

	date.year = static_cast<uint16_t>(year);
	return date;
}

// Are these two values semantically equal for this field?
static bool values_match(fact_field field, const std::string &a,
			 const std::string &b)
{
	switch (field) {
	case fact_field::name:
		return normalize_name(a) == normalize_name(b);

	case fact_field::date_of_birth:
	case fact_field::expiration:
		return normalize_date(a) == normalize_date(b);

	// Default: exact match
	default:
		return a == b;
	}
}

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

	// 5. Nationality
	cf.nationality = obs.entrant_docs.nationality;

	// 6. Recompute discrepancies
	cf.discrepancies = compute_discrepancies(cf.facts);
}

void update_day(inspector &ins, const date_t &scanned_date)
{
	if (ins.current_date != scanned_date) {
		ins.rules.clear();
		ins.rules_loaded = false;
		ins.current_date = scanned_date;

		ins.current_entrant = {};
		ins.entrant_case = {};

		ins.last_printed_rules = "";
		ins.last_advice = "";

		char buf[64];
		(void)sprintf(buf, "=== NEW DAY: %u.%u.%u ===\n",
			      (unsigned)ins.current_date.day,
			      (unsigned)ins.current_date.month,
			      (unsigned)ins.current_date.year);
		ins.inform_player(buf);
	}
}

void update_entrant(inspector &ins, int scanned_entrant_count)
{
	if (ins.current_entrant != scanned_entrant_count) {
		ins.current_entrant = scanned_entrant_count;
		ins.entrant_case = {};

		ins.last_printed_rules = "";
		ins.last_advice = "";

		char buf[64];
		(void)sprintf(buf, "--- Entrant #%i ---\n",
			      ins.current_entrant);
		ins.inform_player(buf);
	}
}

static std::string get_purpose(const case_file &cf)
{
	auto it = cf.facts.cells.find(fact_field::purpose);
	if (it == cf.facts.cells.end())
		return "";

	for (auto &[src, val] : it->second) {
		if (!val.empty())
			return val;
	}
	return "";
}

static std::string get_district(const case_file &cf)
{
	auto it = cf.facts.cells.find(fact_field::district);
	if (it == cf.facts.cells.end())
		return "";

	for (auto &[src, val] : it->second) {
		if (!val.empty())
			return val;
	}
	return "";
}

static bool condition_met(condition c, const case_file &cf)
{
	switch (c) {
	case condition::always:
		return true;

	// === NATIONALITY-BASED (from passport) ===
	case condition::is_citizen:
		return cf.nationality.has_value() &&
		       *cf.nationality == country::arstotzka;

	case condition::is_foreigner:
		return cf.nationality.has_value() &&
		       *cf.nationality != country::arstotzka;

	case condition::is_kolechian:
		return cf.nationality.has_value() &&
		       *cf.nationality == country::kolechia;

	case condition::from_impor:
		return cf.nationality.has_value() &&
		       *cf.nationality == country::impor;

	case condition::from_united_federation:
		return cf.nationality.has_value() &&
		       *cf.nationality == country::united_federation;

	case condition::is_arstotzkan:
		return cf.nationality.has_value() &&
		       *cf.nationality == country::arstotzka;

	// === PURPOSE-BASED (from entry permit, access permit, transcript) ===
	case condition::is_worker:
		return get_purpose(cf) == "WORK";

	case condition::is_diplomat:
		return get_purpose(cf) == "DIPLOMAT";

	case condition::is_asylum_seeker:
		return get_purpose(cf) == "ASYLUM";

	// === LOCATION-BASED (from ID card) ===
	case condition::is_altan_resident:
		return get_district(cf) == "ALTAN";
	}
	return false;
}

std::vector<rule> get_applicable_rules(const std::set<rule> &active,
				       const case_file &cf)
{
	std::vector<rule> applicable;

	for (const rule_def &def : all_rules) {
		if (!active.contains(def.id))
			continue; // not active today
		if (!condition_met(def.when, cf))
			continue; // doesn't apply to this entrant
		applicable.push_back(def.id);
	}

	return applicable;
}

static const rule_def &get_rule_def(rule r)
{
	for (const auto &rd : all_rules) {
		if (rd.id == r) {
			return rd;
		}
	}
	unreachable();
}

enum class rule_status { satisfied, uncertain, not_satisfied };

rule_status check_rule_status(rule r, const case_file& cf) {
    const rule_def& def = get_rule_def(r);
    
    switch (def.what) {
    case effect::require_doc:
        return cf.docs_seen.contains(def.target_doc) 
            ? rule_status::satisfied 
            : rule_status::not_satisfied;
    
    case effect::check_expiration:
        // TODO: check no expired docs
        return rule_status::satisfied;
    
    case effect::search_entrant:
        // If weight mismatch or other suspicion → uncertain
        /*if (has_weight_mismatch(cf) || has_suspicion(cf)) {
            return rule_status::uncertain;
        }*/
        return rule_status::satisfied;
    
    case effect::prohibit_entry:
        return rule_status::not_satisfied;
    
    case effect::confiscate_doc:
        return rule_status::uncertain;
    }
    return rule_status::not_satisfied;
}

void print_applicable_rules(inspector &ins, const case_file &cf)
{
    auto applicable = get_applicable_rules(ins.rules, cf);

    printf("=== APPLICABLE RULES ===\n");
    for (rule r : applicable) {
        const rule_def &def = get_rule_def(r);

        const char* prefix;
	switch (check_rule_status(r, cf)) {
	case rule_status::satisfied:     prefix = "✓"; break;
	case rule_status::uncertain:     prefix = "?"; break;
	case rule_status::not_satisfied: prefix = " "; break;
	}

        const char *effect_str = "";
        switch (def.what) {
        case effect::require_doc:   effect_str = "REQUIRE";   break;
        case effect::prohibit_entry: effect_str = "PROHIBIT"; break;
        case effect::confiscate_doc: effect_str = "CONFISCATE"; break;
        case effect::search_entrant: effect_str = "SEARCH";   break;
        case effect::check_expiration: effect_str = "CHECK_EXP"; break;
        }
        
        printf(" %s [%s] %s\n", prefix, effect_str,
               magic_enum::enum_name(r).data());
    }
    printf("========================\n");
}
constexpr fact_field classification_fields[] = {
	fact_field::nationality, // → is_citizen, is_foreigner, is_kolechian
	fact_field::purpose, // → is_worker, is_diplomat
	fact_field::district, // → is_altan_resident
};

static std::optional<discrepancy>
get_discrepancy_for_field(const fact_table &facts, fact_field field)
{
	const auto &it = facts.cells.find(field);
	if (it == facts.cells.end())
		return std::nullopt;

	std::vector<std::pair<doc_type, std::string> > cells{};
	for (const auto &match : it->second)
		cells.emplace_back(match);

	if (cells.size() < 2)
		return std::nullopt;

	// Compare all values against the first one
	const std::string &first = cells[0].second;
	for (size_t i = 1; i < cells.size(); i++) {
		if (!values_match(field, first, cells[i].second)) {
			return discrepancy{ field, cells };
		}
	}

	return std::nullopt;
}

std::vector<discrepancy> check_classification_conflicts(const case_file &cf)
{
	std::vector<discrepancy> conflicts;

	for (fact_field f : classification_fields) {
		auto d = get_discrepancy_for_field(cf.facts, f);
		if (d.has_value()) {
			conflicts.push_back(*d);
		}
	}

	return conflicts;
}

void print_conflict(const discrepancy &conflict)
{
	printf("CONFLICT [%s]:\n",
	       magic_enum::enum_name(conflict.field).data());
	for (const auto &[src, val] : conflict.values) {
		printf("  %s says: %s\n", magic_enum::enum_name(src).data(),
		       val.c_str());
	}
}

std::set<doc_type> compute_required_docs(const std::vector<rule> &applicable)
{
	std::set<doc_type> required;
	for (rule r : applicable) {
		const rule_def &def = get_rule_def(r);
		if (def.what == effect::require_doc) {
			required.insert(def.target_doc);
		}
	}
	return required;
}

std::set<doc_type> compute_missing_docs(const std::set<doc_type> &required,
					const std::set<doc_type> &seen)
{
	std::set<doc_type> missing;
	for (doc_type d : required) {
		if (!seen.contains(d)) {
			missing.insert(d);
		}
	}
	return missing;
}

std::vector<discrepancy>
compute_discrepancies_for_docs(const fact_table &facts,
                               const std::set<doc_type> &relevant_docs)
{
    std::vector<discrepancy> out;

    for (const auto &[field, sources] : facts.cells) {
        // Filter to only relevant docs
        std::vector<std::pair<doc_type, std::string>> filtered;
        for (const auto &[src, val] : sources) {
            if (relevant_docs.contains(src)) {
                filtered.emplace_back(src, val);
            }
        }

        // Need at least 2 sources to have a discrepancy
        if (filtered.size() < 2)
            continue;

        // Compare all against first using values_match()
        const std::string& first = filtered[0].second;
        bool has_mismatch = false;
        for (size_t i = 1; i < filtered.size(); i++) {
            if (!values_match(field, first, filtered[i].second)) {
                has_mismatch = true;
                break;
            }
        }

        if (has_mismatch) {
            discrepancy d;
            d.field = field;
            for (const auto &[src, val] : filtered) {
                d.values.push_back({src, val});
            }
            out.push_back(d);
        }
    }

    return out;
}

void inspector_step(inspector &ins, const game_screen &screen,
		    const resources_ctx &ctx)
{
	auto obs = observe_frame(screen, ctx);

	if (!obs.booth_ok) {
		ins.inform_player("Booth is not in a valid state");
		return;
	}

	// react to booth info
	update_day(ins, obs.date);
	update_entrant(ins, obs.entrant_count);

	// === add THIS: rulebook loading ===
	if (!ins.rules_loaded) {
		if (obs.rulebook_ok) {
			ins.rules = obs.rules;
			ins.rules_loaded = true;
			ins.inform_player("Rulebook loaded");
		} else {
			ins.inform_player("NEED: show rulebook");
			return;
		}
	}

	// update understanding of entrant
	update_case(ins.entrant_case, obs);

	// 1. Check classification conflicts FIRST
	auto conflicts = check_classification_conflicts(ins.entrant_case);
	if (!conflicts.empty()) {
		ins.inform_player("CONFLICT in classification data");
		for (const auto &conflict : conflicts) {
			print_conflict(conflict);
		}
		return; // Can't proceed without knowing who this entrant is
	}

	// 2. Now safe to get applicable rules
	auto applicable = get_applicable_rules(ins.rules, ins.entrant_case);
	print_applicable_rules(ins, ins.entrant_case);

	// 3. Required docs from applicable rules
	auto required = compute_required_docs(applicable);

	// 4. What's missing?
	auto missing =
		compute_missing_docs(required, ins.entrant_case.docs_seen);

	// 5. Discrepancies (only on required docs)
	auto discreps = compute_discrepancies_for_docs(ins.entrant_case.facts,
						       required);

	// 6. OUTPUT (priority order)
	std::string advice;
	if (!missing.empty()) {
		advice = ">>> NEED DOCS\n";
		for (doc_type d : missing) {
			advice += "    - ";
			advice += magic_enum::enum_name(d);
			advice += "\n";
		}
	} else if (!discreps.empty()) {
    advice = ">>> DISCREPANCY\n";
    for (const auto& d : discreps) {
        advice += "    [";
        advice += magic_enum::enum_name(d.field);
        advice += "]\n";
        for (const auto& [src, val] : d.values) {
            advice += "      ";
            advice += magic_enum::enum_name(src);
            advice += " = \"";
            advice += val;
            advice += "\"\n";
        }
    }	} else {
		advice = "FINE";
	}

	if (advice != ins.last_advice) {
		ins.last_advice = advice;
		printf("%s\n", advice.c_str());
	}
}
