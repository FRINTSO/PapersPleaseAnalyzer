#include "checks.h"
#include "paplease/compiler.h"
#include "paplease/documents.h"
#include "paplease/inspector.h"
#include <vector>

// Normalize name for comparison
static std::string normalize_name(const std::string &raw)
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
		return normalize_date(a) == normalize_date(b);

	// Default: exact match
	default:
		return a == b;
	}
}

source doc_to_source(doc_type d)
{
	{
		switch (d) {
		case doc_type::passport:
			return source::passport;
		case doc_type::entry_permit:
			return source::entry_permit;
		case doc_type::work_pass:
			return source::work_pass;
		case doc_type::identity_card:
			return source::id_card;
		case doc_type::access_permit:
			return source::access_permit;
		case doc_type::certificate_of_vaccination:
			return source::vaccination_cert;
		case doc_type::grant_of_asylum:
			return source::asylum_grant;
		case doc_type::identity_supplement:
			return source::identity_supplement;
		default:
			unreachable();
		}
	}
}

static response get_response_for_mismatch(fact_field field)
{
	switch (field) {
	// Fraud indicators → detain
	case fact_field::passport_number:
		return response::detain;

	// Identity verification needed → fingerprint
	case fact_field::date_of_birth:
		return response::fingerprint;

	// Minor discrepancy → interrogate
	case fact_field::sex:
	case fact_field::name:
	case fact_field::height_cm:
	case fact_field::weight_kg:
	default:
		return response::interrogate;
	}
}

std::vector<problem> find_problems(const case_file &cf, const date_t &today)
{
	std::vector<problem> out;

	// 1. Identity mismatches (from fact_table)
	for (const auto &[field, sources] : cf.facts.cells) {
		if (sources.size() < 2)
			continue;

		// Check if values differ
		auto it = sources.begin();
		const std::string &first = it->second;
		bool mismatch = false;
		for (++it; it != sources.end(); ++it) {
			if (!values_match(field, first, it->second)) {
				mismatch = true;
				break;
			}
		}

		if (mismatch) {
			problem p;
			p.kind = problem_kind::identity_mismatch;
			p.action = get_response_for_mismatch(field);
			for (const auto &[doc, val] : sources) {
				p.claims.push_back({ doc_to_source(doc), val });
			}
			out.push_back(p);
		}
	}

	// 2. Expired documents
	for (const auto &[doc, exp] : cf.expirations) {
		if (exp < today) {
			problem p;
			p.kind = problem_kind::expired;
			p.claims.push_back(
				{ doc_to_source(doc), format_date(exp) });
			p.claims.push_back(
				{ source::booth_date, format_date(today) });
			p.action = response::deny;
			out.push_back(p);
		}
	}

	// 3. Weight mismatch (when you have booth scale data)
	// if (cf.booth_weight.has_value()) {
	//     for weight in cf.weights...
	// }

	// 4. Height mismatch
	// ...

	return out;
}
