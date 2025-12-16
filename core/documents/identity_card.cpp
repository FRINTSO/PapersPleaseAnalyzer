#include <cassert>
#include <paplease/documents.h>
#include <paplease/geometry.h>
#include <paplease/ocr.h>
#include "parse_helpers.h"

static constexpr rectangle BOX_DISTRICT = {12, 22, 232, 12};
static constexpr rectangle BOX_NAME     = {100, 40, 144, 30};
static constexpr rectangle BOX_DOB      = {130, 80, 114, 12};
static constexpr rectangle BOX_HEIGHT   = {130, 100, 114, 12};
static constexpr rectangle BOX_WEIGHT   = {130, 120, 114, 12};

bool parse_identity_card(identity_card_data &out, const doc &document,
			 const resources_ctx &ctx)
{
	assert(document.type == doc_type::identity_card);
	cv::Mat binary = preprocess_document(document.pixels);
	typeface tf = typeface_for(doc_type::identity_card);

	std::string tmp;

	if (!extract_field(out.district, binary, BOX_DISTRICT, tf, ctx))
		return false;

	if (!extract_field(out.name, binary, BOX_NAME, tf, ctx))
		return false;

	if (!extract_field(tmp, binary, BOX_DOB, tf, ctx))
		return false;
	if (!parse_date(out.date_of_birth, tmp))
		return false;

	if (!extract_field(tmp, binary, BOX_HEIGHT, tf, ctx))
		return false;
	if (!parse_height(out.height_cm, tmp))
		return false;

	if (!extract_field(tmp, binary, BOX_WEIGHT, tf, ctx))
		return false;
	if (!parse_weight(out.weight_kg, tmp))
		return false;

	return true;
}
