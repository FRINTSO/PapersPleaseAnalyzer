#include <cassert>
#include <paplease/documents.h>
#include <paplease/geometry.h>
#include <paplease/ocr.h>
#include "parse_helpers.h"

static constexpr rectangle BOX_NAME           = {74, 182, 214, 12};
static constexpr rectangle BOX_PASSPORT_NUM   = {98, 206, 190, 12};
static constexpr rectangle BOX_ISSUING_COUNTRY= {68, 8, 140, 12};
static constexpr rectangle BOX_COUNTRY_LIST   = {42, 304, 240, 56};

bool parse_diplomatic_authorization(diplomatic_authorization_data &out,
				    const doc &document,
				    const resources_ctx &ctx)
{
	assert(document.type == doc_type::diplomatic_authorization);
	cv::Mat binary = preprocess_document(document.pixels);
	typeface tf = typeface_for(doc_type::diplomatic_authorization);

	if (!extract_field(out.name, binary, BOX_NAME, tf, ctx))
		return false;

	if (!extract_field(out.passport_number, binary, BOX_PASSPORT_NUM, tf,
			   ctx))
		return false;

	if (!extract_field(out.issuing_country, binary, BOX_ISSUING_COUNTRY, tf,
			   ctx))
		return false;

	if (!extract_field(out.access_countries, binary, BOX_COUNTRY_LIST, tf,
			   ctx))
		return false;

	return true;
}
