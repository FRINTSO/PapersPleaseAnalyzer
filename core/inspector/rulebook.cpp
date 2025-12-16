#ifndef _PAPLEASE_CORE_INSPECTOR_RULEBOOK_H
#define _PAPLEASE_CORE_INSPECTOR_RULEBOOK_H
#include "opencv2/core/mat.hpp"
#include "opencv2/imgproc.hpp"
#include <algorithm>
#include <cassert>

#include <cctype>
#include <paplease/documents.h>
#include <paplease/ocr.h>
#include <paplease/geometry.h>
#include <paplease/inspector.h>

#include "rulebook.h"

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

static bool apply_rule(day_rules &out, std::string_view rule)
{
	if (rule == "all documents must be current") {
		out.all_docs_must_be_current = true;
	} else if (rule == "all kolechians must be searched") {
		out.require_search_kolechians = true;
	} else if (rule == "arstotzkan citizens must have an id card") {
		out.require_identity_card = true;
	} else if (rule == "arstotzkan citizens only") {
		out.require_arstotzkan_passport = true;
	} else if (rule == "asylum seekers must have a grant") {
		out.require_asylum_grant = true;
	} else if (rule == "citizens must have an id card") {
		out.require_identity_card = true;
	} else if (rule == "confiscate all arstotzkan passports") {
		out.confiscate_arstotzkan_passports = true;
	} else if (rule ==
		   "confiscate arstotzkan passports belonging to altan residents") {
		out.confiscate_altan_passports = true;
	} else if (rule == "diplomats require authorization") {
		out.require_diplomatic_authorization = true;
	} else if (rule == "entrant must have a passport") {
		out.require_passport = true;
	} else if (rule == "entrant must have polio vaccine cert") {
		out.require_polio_vaccination = true;
	} else if (rule == "foreigners require an access permit") {
		out.require_access_permit = true;
	} else if (rule == "foreigners require an entry permit") {
		out.require_entry_permit = true;
	} else if (rule == "foreigners require an entry ticket") {
		out.require_entry_ticket = true;
	} else if (rule == "foreigners require an id supplement") {
		out.require_identity_supplement = true;
	} else if (rule == "no entry from united federation") {
		out.prohibit_united_federation = true;
	} else if (rule == "no entry from impor") {
		out.prohibit_impor = true;
	} else if (rule == "no weapons or contraband") {
		out.prohibit_weapons_and_contraband = true;
	} else if (rule == "workers must have a work pass") {
		out.require_work_pass = true;
	} else if (rule.empty()) {
		// empty rule
	} else {
		return false; // unknown rule
	}

	return true; // rule applied
}

bool parse_rulebook(day_rules &out, const doc &rulebook,
		    const resources_ctx &ctx)
{
	constexpr typeface tf = typeface_for(doc_type::rulebook);
	cv::Mat binary_rulebook = preprocess_rulebook(rulebook.pixels);
	for (int i = 0; i < MAX_RULES; i++) {
		rectangle rule_box = nth_rule_box(i);
		std::string rule;
		cv::Mat rule_region = binary_rulebook(rule_box.to_cv());
		if (!extract_text_strict(rule, rule_region, tf, ctx))
			return false;

		std::transform(rule.begin(), rule.end(), rule.begin(), ::tolower);

		if (!apply_rule(out, rule))
			return false;
	}
	return true;
}
#endif // _PAPLEASE_CORE_INSPECTOR_RULEBOOK_H
