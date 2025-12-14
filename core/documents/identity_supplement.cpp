#include <cassert>
#include <paplease/documents.h>
#include <paplease/geometry.h>
#include <paplease/ocr.h>
#include "parse_helpers.h"

static constexpr rectangle BOX_HEIGHT        = {46, 64, 114, 16};
static constexpr rectangle BOX_WEIGHT        = {46, 86, 114, 16};
static constexpr rectangle BOX_PHYSICAL_DESC = {20, 132, 142, 48};
static constexpr rectangle BOX_EXPIRATION    = {52, 278, 108, 16};

bool parse_identity_supplement(identity_supplement_data &out, const doc &document)
{
	assert(document.variant == doc_variant::identity_supplement);
	cv::Mat binary = preprocess_document(document.pixels);
	typeface tf = typeface_for(doc_type::identity_supplement);

	std::string tmp;

	if (!extract_field(tmp, binary, BOX_HEIGHT, tf))
		return false;
	if (!parse_height(out.height_cm, tmp))
		return false;

	if (!extract_field(tmp, binary, BOX_WEIGHT, tf))
		return false;
	if (!parse_weight(out.weight_kg, tmp))
		return false;

	if (!extract_field(out.physical_desc, binary, BOX_PHYSICAL_DESC, tf))
		return false;

	if (!extract_field(tmp, binary, BOX_EXPIRATION, tf))
		return false;
	if (!parse_date(out.expiration, tmp))
		return false;

	return true;
}
