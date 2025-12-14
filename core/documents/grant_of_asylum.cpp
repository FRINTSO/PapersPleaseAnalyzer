#include <cassert>
#include <paplease/documents.h>
#include <paplease/geometry.h>
#include <paplease/ocr.h>
#include "parse_helpers.h"

static constexpr rectangle BOX_NAME           = {144, 108, 154, 34};
static constexpr rectangle BOX_ISSUING_COUNTRY= {182, 156, 116, 16};
static constexpr rectangle BOX_PASSPORT_NUM   = {182, 174, 116, 16};
static constexpr rectangle BOX_DOB            = {182, 192, 116, 16};
static constexpr rectangle BOX_HEIGHT         = {182, 210, 116, 16};
static constexpr rectangle BOX_WEIGHT         = {182, 228, 116, 16};
static constexpr rectangle BOX_EXPIRATION     = {156, 326, 100, 16};

bool parse_grant_of_asylum(grant_of_asylum_data &out, const doc &document)
{
	assert(document.variant == doc_variant::grant_of_asylum);
	cv::Mat binary = preprocess_document(document.pixels);
	typeface tf = typeface_for(doc_type::grant_of_asylum);

	std::string tmp;

	if (!extract_field(out.name, binary, BOX_NAME, tf))
		return false;

	if (!extract_field(out.issuing_country, binary, BOX_ISSUING_COUNTRY, tf))
		return false;

	if (!extract_field(out.passport_number, binary, BOX_PASSPORT_NUM, tf))
		return false;

	if (!extract_field(tmp, binary, BOX_DOB, tf))
		return false;
	if (!parse_date(out.date_of_birth, tmp))
		return false;

	if (!extract_field(tmp, binary, BOX_HEIGHT, tf))
		return false;
	if (!parse_height(out.height_cm, tmp))
		return false;

	if (!extract_field(tmp, binary, BOX_WEIGHT, tf))
		return false;
	if (!parse_weight(out.weight_kg, tmp))
		return false;

	if (!extract_field(tmp, binary, BOX_EXPIRATION, tf))
		return false;
	if (!parse_date(out.expiration, tmp))
		return false;

	return true;
}
