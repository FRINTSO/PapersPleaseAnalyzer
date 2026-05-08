#include <optional>
#include <string>
#include <vector>

#include <magic_enum/magic_enum.hpp>

#include <paplease/compiler.h>
#include <paplease/date.h>
#include <paplease/documents.h>
#include <paplease/inspector.h>
#include <paplease/observation.h>
#include <paplease/rules.h>
#include <paplease/vision.h>

#include "case_file.h"
#include "checks.h"

static void update_day(inspector &ins, const date_t &scanned_date)
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

static void update_entrant(inspector &ins, int scanned_entrant_count,
			   std::optional<int> scanned_booth_weight)
{
	if (ins.current_entrant != scanned_entrant_count) {
		ins.current_entrant = scanned_entrant_count;
		ins.entrant_case = {};
		ins.entrant_case.booth_weight_kg = scanned_booth_weight;

		ins.last_printed_rules = "";
		ins.last_advice = "";

		char buf[64];
		(void)sprintf(buf, "--- Entrant #%i ---\n",
			      ins.current_entrant);
		ins.inform_player(buf);
	}
}

static bool condition_met(condition c, const case_file &cf)
{
	switch (c) {
	case condition::always:
		return true;

	// === NATIONALITY-BASED (from passport) ===
	case condition::is_arstotzkan:
	case condition::is_citizen:
		return cf.nationality.has_value() &&
		       *cf.nationality == country::arstotzka;

	case condition::is_foreigner:
		return (cf.nationality.has_value() &&
			*cf.nationality != country::arstotzka) ||
		       cf.docs_seen.contains(doc_type::entry_permit) ||
		       cf.docs_seen.contains(
			       doc_type::access_permit); // TODO: Add remaining foreigner docs here

	case condition::is_kolechian:
		return cf.nationality.has_value() &&
		       *cf.nationality == country::kolechia;

	case condition::from_impor:
		return cf.nationality.has_value() &&
		       *cf.nationality == country::impor;

	case condition::from_united_federation:
		return cf.nationality.has_value() &&
		       *cf.nationality == country::united_federation;

	// === PURPOSE-BASED (from entry permit, access permit, transcript) ===
	case condition::is_worker:
		return get_purpose(cf) == "WORK" ||
		       cf.docs_seen.contains(doc_type::work_pass);

	case condition::is_diplomat:
		return get_purpose(cf) == "DIPLOMAT" ||
		       cf.docs_seen.contains(
			       doc_type::diplomatic_authorization);

	case condition::is_asylum_seeker:
		return get_purpose(cf) == "ASYLUM" ||
		       cf.docs_seen.contains(doc_type::grant_of_asylum);

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

static bool has_expirations(const case_file &cf, const date_t &today)
{
	for (const auto &[a, b] : cf.expirations) {
		if (b < today) {
			return true;
		}
	}
	return false;
}

static bool has_weight_mismatch(const case_file &cf, std::optional<int> booth_weight)
{
	if (!booth_weight.has_value())
		return false; // Can't check without booth data

	const auto& it = cf.facts.cells.find(fact_field::weight_kg);
	if (it == cf.facts.cells.end())
		return false; // undecided, no documents mentioning weight yet
	
	for (const auto& k : it->second) {
		int document_weigth = std::stoi(k.second);
		if (document_weigth != booth_weight)
			return true;
	}

	return false;
}

static rule_status check_rule_status(rule r, const case_file &cf,
				     const date_t &today, std::optional<int> booth_weight)
{
	const rule_def &def = get_rule_def(r);

	switch (def.what) {
	case effect::require_doc:
		return cf.docs_seen.contains(def.target_doc) ?
			       rule_status::satisfied :
			       rule_status::uncertain;

	case effect::check_expiration:
		// TODO: check no expired docs
		if (has_expirations(cf, today)) {
			return rule_status::not_satisfied;
		}
		return rule_status::satisfied;

	case effect::search_entrant:
		// If weight mismatch or other suspicion → uncertain
		if (has_weight_mismatch(cf, booth_weight)) {
			return rule_status::uncertain;
		}
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

		const char *prefix;
		switch (check_rule_status(r, cf, ins.current_date, ins.entrant_case.booth_weight_kg)) {
		case rule_status::satisfied:
			prefix = "✓";
			break;
		case rule_status::uncertain:
			prefix = "?";
			break;
		case rule_status::not_satisfied:
			prefix = "✗";
			break;
		}

		const char *effect_str = "";
		switch (def.what) {
		case effect::require_doc:
			effect_str = "REQUIRE";
			break;
		case effect::prohibit_entry:
			effect_str = "PROHIBIT";
			break;
		case effect::confiscate_doc:
			effect_str = "CONFISCATE";
			break;
		case effect::search_entrant:
			effect_str = "SEARCH";
			break;
		case effect::check_expiration:
			effect_str = "CHECK_EXP";
			break;
		}

		printf(" %s [%s] %s\n", prefix, effect_str,
		       magic_enum::enum_name(r).data());
	}
	printf("========================\n");
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

// Helper: filter problems by action
static std::vector<problem>
filter_by_action(const std::vector<problem> &problems,
		 std::initializer_list<response> actions)
{
	std::vector<problem> out;
	for (const auto &p : problems) {
		for (response a : actions) {
			if (p.action == a) {
				out.push_back(p);
				break;
			}
		}
	}
	return out;
}

// Helper: format problems for output
static std::string format_problems(const std::vector<problem> &problems,
				   const char *header)
{
	std::string out = header;
	for (const auto &p : problems) {
		out += "    [";
		out += magic_enum::enum_name(p.kind);
		out += "]\n";
		for (const auto &c : p.claims) {
			out += "      ";
			out += magic_enum::enum_name(c.who);
			out += " = \"";
			out += c.what;
			out += "\"\n";
		}
	}
	return out;
}

// Helper: format missing docs
static std::string format_missing(const std::set<doc_type> &missing)
{
	std::string out = ">>> NEED DOCS\n";
	for (doc_type d : missing) {
		out += "    - ";
		out += magic_enum::enum_name(d);
		out += "\n";
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
	update_entrant(ins, obs.entrant_count, obs.entrant_weight);

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

	// update_case: just merge
	auto new_docs = update_case(ins.entrant_case, obs);

	// 2. Applicable rules + required docs
	auto applicable = get_applicable_rules(ins.rules, ins.entrant_case);
	auto required = compute_required_docs(applicable);
	auto missing =
		compute_missing_docs(required, ins.entrant_case.docs_seen);

	// 3. Find ALL problems in one place
	ins.entrant_case.problems = find_problems(ins.entrant_case, obs.date);

	// 4. Print rules
	print_applicable_rules(ins, ins.entrant_case);

	// Separate hard problems from soft ones
	auto hard_problems =
		filter_by_action(ins.entrant_case.problems,
				 { response::deny, response::detain });
	auto soft_problems = filter_by_action(ins.entrant_case.problems,
					      { response::interrogate,
						response::fingerprint });

	// Priority order
	std::string advice;
	if (!hard_problems.empty()) {
		// DENY/DETAIN - game over, no point asking for more docs
		advice = format_problems(hard_problems, ">>> DENY\n");
	} else if (!missing.empty()) {
		// Need more docs before we can decide
		advice = format_missing(missing);
	} else if (!soft_problems.empty()) {
		// Minor issues - interrogate/fingerprint
		advice = format_problems(soft_problems, ">>> VERIFY\n");
	} else {
		advice = ">>> APPROVED ✓\n";
	}

	if (advice != ins.last_advice) {
		ins.last_advice = advice;
		printf("%s\n", advice.c_str());
	}
}
