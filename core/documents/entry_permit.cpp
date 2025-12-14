#include <cassert>
#include <paplease/documents.h>
#include <paplease/geometry.h>
#include <paplease/ocr.h>
#include "parse_helpers.h"

static constexpr rectangle BOX_NAME         = {30, 182, 238, 12};
static constexpr rectangle BOX_PASSPORT_NUM = {30, 248, 238, 12};
static constexpr rectangle BOX_PURPOSE      = {102, 280, 166, 12};
static constexpr rectangle BOX_DURATION     = {102, 310, 166, 12};
static constexpr rectangle BOX_EXPIRATION   = {102, 340, 166, 12};

bool parse_entry_permit(entry_permit_data &out, const doc &document)
{
	assert(document.variant == doc_variant::entry_permit);
	cv::Mat binary = preprocess_document(document.pixels);
	typeface tf = typeface_for(doc_type::entry_permit);

	std::string tmp;

	if (!extract_field(out.name, binary, BOX_NAME, tf))
		return false;

	if (!extract_field(out.passport_number, binary, BOX_PASSPORT_NUM, tf))
		return false;

	if (!extract_field(out.purpose, binary, BOX_PURPOSE, tf))
		return false;

	if (!extract_field(out.duration, binary, BOX_DURATION, tf))
		return false;

	if (!extract_field(tmp, binary, BOX_EXPIRATION, tf))
		return false;
	if (!parse_date(out.expiration, tmp))
		return false;

	return true;
}
