#include <cassert>
#include <paplease/documents.h>
#include <paplease/geometry.h>
#include <paplease/ocr.h>
#include "parse_helpers.h"

static constexpr rectangle BOX_NAME       = {74, 134, 204, 12};
static constexpr rectangle BOX_WORK_FIELD = {74, 164, 204, 12};
static constexpr rectangle BOX_END_DATE   = {74, 194, 204, 12};

bool parse_work_pass(work_pass_data &out, const doc &document,
		     const resources_ctx &ctx)
{
	assert(document.type == doc_type::work_pass);
	cv::Mat binary = preprocess_document(document.pixels);
	typeface tf = typeface_for(doc_type::work_pass);

	std::string tmp;

	if (!extract_field(out.name, binary, BOX_NAME, tf, ctx))
		return false;

	if (!extract_field(out.work_field, binary, BOX_WORK_FIELD, tf, ctx))
		return false;

	if (!extract_field(tmp, binary, BOX_END_DATE, tf, ctx))
		return false;
	if (!parse_date(out.end_date, tmp))
		return false;

	return true;
}
