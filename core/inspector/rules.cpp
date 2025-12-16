#include <algorithm>
#include <cassert>
#include <cctype>

#include <opencv2/imgproc.hpp>
#include <opencv2/core/mat.hpp>

#include <paplease/documents.h>
#include <paplease/geometry.h>
#include <paplease/inspector.h>
#include <paplease/ocr.h>
#include <paplease/rules.h>

static constexpr int RULE_COL1_X = 32;
static constexpr int RULE_COL2_X = 274;
static constexpr int TOP_PIXEL_Y = 50;
static constexpr int RULE_BOX_WIDTH = 188;
static constexpr int RULE_BOX_HEIGHT = 38;
static constexpr int SPACE_TO_NEXT_RULE = 48;
static constexpr int RULE_ROWS = 5;

static constexpr int MAX_RULES = 10;

static rectangle nth_rule_box(int n)
{
	constexpr int COLUMN_COUNT = 2;
	assert(0 <= n && n < COLUMN_COUNT * RULE_ROWS);

	int column = n / RULE_ROWS;
	int row = n % RULE_ROWS;

	int x = (column == 0 ? RULE_COL1_X : RULE_COL2_X);
	int y = TOP_PIXEL_Y + row * SPACE_TO_NEXT_RULE;

	return rectangle{ x, y, RULE_BOX_WIDTH, RULE_BOX_HEIGHT };
}

static cv::Mat preprocess_rulebook(const cv::Mat &rulebook)
{
	cv::Mat grayscale;
	cv::cvtColor(rulebook, grayscale, cv::COLOR_BGR2GRAY);

	cv::Mat thresh;
	cv::threshold(grayscale, thresh, 157, 255, cv::THRESH_BINARY);
	return thresh;
}

static bool parse_rule(rule &out, std::string_view text)
{
	if (text == "entrant must have a passport")
		out = rule::entrant_needs_passport;
	else if (text == "entrant must have polio vaccine cert")
		out = rule::entrant_needs_polio_vaccine;
	else if (text == "citizens must have an id card" ||
		 text == "arstotzkan citizens must have an id card")
		out = rule::citizens_need_id_card;
	else if (text == "foreigners require an entry permit")
		out = rule::foreigners_need_entry_permit;
	else if (text == "foreigners require an entry ticket")
		out = rule::foreigners_need_entry_ticket;
	else if (text == "foreigners require an access permit")
		out = rule::foreigners_need_access_permit;
	else if (text == "foreigners require an id supplement")
		out = rule::foreigners_need_id_supplement;
	else if (text == "workers must have a work pass")
		out = rule::workers_need_work_pass;
	else if (text == "diplomats require authorization")
		out = rule::diplomats_need_authorization;
	else if (text == "asylum seekers must have a grant")
		out = rule::asylum_seekers_need_grant;
	else if (text == "all documents must be current")
		out = rule::all_docs_must_be_current;
	else if (text == "arstotzkan citizens only")
		out = rule::arstotzkan_citizens_only;
	else if (text == "no entry from impor")
		out = rule::no_entry_from_impor;
	else if (text == "no entry from united federation")
		out = rule::no_entry_from_united_federation;
	else if (text == "no weapons or contraband")
		out = rule::no_weapons_or_contraband;
	else if (text == "all kolechians must be searched")
		out = rule::kolechians_must_be_searched;
	else if (text == "confiscate all arstotzkan passports")
		out = rule::confiscate_arstotzkan_passports;
	else if (text ==
		 "confiscate arstotzkan passports belonging to altan residents")
		out = rule::confiscate_altan_passports;
	else if (text.empty())
		return true;
	else
		return false;

	return true;
}

bool parse_rules(std::set<rule> &out, const doc &rulebook,
		 const resources_ctx &ctx)
{
	constexpr typeface tf = typeface_for(doc_type::rulebook);
	cv::Mat binary_rulebook = preprocess_rulebook(rulebook.pixels);
	for (int i = 0; i < MAX_RULES; i++) {
		rectangle rule_box = nth_rule_box(i);
		std::string rule_text;
		cv::Mat rule_region = binary_rulebook(rule_box.to_cv());
		if (!extract_text_strict(rule_text, rule_region, tf, ctx))
			return false;

		std::transform(rule_text.begin(), rule_text.end(),
			       rule_text.begin(), ::tolower);

		rule r;
		if (!parse_rule(r, rule_text))
			return false;

		out.insert(r);
	}
	return true;
}

