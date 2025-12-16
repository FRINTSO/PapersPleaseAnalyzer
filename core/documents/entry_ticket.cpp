#include <cassert>
#include <paplease/documents.h>
#include <paplease/geometry.h>
#include <paplease/ocr.h>
#include "parse_helpers.h"

static constexpr rectangle BOX_VALID_DATE = {176, 64, 90, 16};

bool parse_entry_ticket(entry_ticket_data &out, const doc &document,
			const resources_ctx &ctx)
{
	assert(document.type == doc_type::entry_ticket);
	cv::Mat binary = preprocess_document(document.pixels);
	typeface tf = typeface_for(doc_type::entry_ticket);

	std::string tmp;

	if (!extract_field(tmp, binary, BOX_VALID_DATE, tf, ctx))
		return false;
	if (!parse_date(out.valid_date, tmp))
		return false;

	return true;
}
