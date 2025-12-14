#include <cassert>
#include <paplease/documents.h>
#include <paplease/geometry.h>
#include <paplease/ocr.h>
#include "parse_helpers.h"

static constexpr rectangle BOX_NAME           = {30, 108, 234, 16};
static constexpr rectangle BOX_ISSUING_COUNTRY= {30, 152, 112, 16};
static constexpr rectangle BOX_PASSPORT_NUM   = {156, 152, 116, 16};
static constexpr rectangle BOX_PURPOSE        = {30, 196, 112, 16};
static constexpr rectangle BOX_DURATION       = {156, 196, 112, 16};
static constexpr rectangle BOX_HEIGHT         = {30, 240, 112, 16};
static constexpr rectangle BOX_WEIGHT         = {156, 240, 112, 16};
static constexpr rectangle BOX_PHYSICAL_DESC  = {30, 284, 234, 16};
static constexpr rectangle BOX_EXPIRATION     = {156, 328, 108, 16};

bool parse_access_permit(access_permit_data &out, const doc &document)
{
	assert(document.variant == doc_variant::access_permit);
	cv::Mat binary = preprocess_document(document.pixels);
	typeface tf = typeface_for(doc_type::access_permit);

	std::string tmp;

	if (!extract_field(out.name, binary, BOX_NAME, tf))
		return false;

	if (!extract_field(out.issuing_country, binary, BOX_ISSUING_COUNTRY, tf))
		return false;

	if (!extract_field(out.passport_number, binary, BOX_PASSPORT_NUM, tf))
		return false;

	if (!extract_field(out.purpose, binary, BOX_PURPOSE, tf))
		return false;

	if (!extract_field(out.duration, binary, BOX_DURATION, tf))
		return false;

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
