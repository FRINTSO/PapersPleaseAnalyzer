#include "paplease/date.h"
#include "paplease/inspector.h"
#include <set>
#include <string>

#include <paplease/documents.h>

#include "case_file.h"

// update_case: pure merge, returns newly added docs
std::set<doc_type> update_case(case_file &cf, const observation &obs)
{
	std::set<doc_type> newly_added;

	for (doc_type dt : obs.entrant_docs.visible) {
		if (!cf.docs_seen.contains(dt))
			newly_added.insert(dt);
		cf.docs_seen.insert(dt);
	}

	// Merge identity facts
	for (const auto &[doc, field_map] : obs.entrant_docs.identity_fields) {
		for (auto &[field, value] : field_map) {
			if (!value.empty()) {
				cf.facts.put(field, doc, value);
			}
		}
	}

	// Merge typed per-doc values
	for (const auto &[doc, exp] : obs.entrant_docs.expirations) {
		cf.expirations[doc] = exp;
	}
	// Other per-doc checks...

	// Copy vaccines
	if (obs.entrant_docs.vaccine_count > 0) {
		cf.vaccine_count = obs.entrant_docs.vaccine_count;
		for (int i = 0; i < cf.vaccine_count; i++) {
			cf.vaccines[i] = obs.entrant_docs.vaccines[i];
		}
	}

	// Nationality
	if (obs.entrant_docs.nationality.has_value()) {
		cf.nationality = obs.entrant_docs.nationality;
	}
	cf.booth_weight_kg = obs.entrant_weight;

	return newly_added;
}

// GETTERS
std::string get_purpose(const case_file &cf)
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

std::string get_district(const case_file &cf)
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
