#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "paplease/resources.h"
#include <cassert>
#include <ctime>
#include <paplease/documents.h>
#include <paplease/geometry.h>
#include <paplease/ocr.h>
#include "parse_helpers.h"

static constexpr rectangle BOX_NAME = { 32, 102, 206, 12 };
static constexpr rectangle BOX_PASSPORT_NUM = { 54, 128, 184, 12 };
static constexpr rectangle BOX_VACCINE_1_DATE = { 34, 190, 82, 12 };
static constexpr rectangle BOX_VACCINE_1_NAME = { 118, 190, 118, 12 };
static constexpr rectangle BOX_VACCINE_2_DATE = { 34, 214, 82, 12 };
static constexpr rectangle BOX_VACCINE_2_NAME = { 118, 214, 118, 12 };
static constexpr rectangle BOX_VACCINE_3_DATE = { 34, 238, 82, 12 };
static constexpr rectangle BOX_VACCINE_3_NAME = { 118, 238, 118, 12 };

bool parse_certificate_of_vaccination(certificate_of_vaccination_data &out,
				      const doc &document,
				      const resources_ctx &ctx)
{
	assert(document.type == doc_type::certificate_of_vaccination);
	cv::Mat binary = preprocess_document(document.pixels);
	typeface tf = typeface_for(doc_type::certificate_of_vaccination);

	if (!extract_field(out.name, binary, BOX_NAME, tf, ctx))
		return false;

	if (!extract_field(out.passport_number, binary, BOX_PASSPORT_NUM, tf,
			   ctx))
		return false;

	// Vaccine rows - only parse date if name is present
	static constexpr rectangle VAX_NAME_BOXES[] = {
		BOX_VACCINE_1_NAME, BOX_VACCINE_2_NAME, BOX_VACCINE_3_NAME
	};
	static constexpr rectangle VAX_DATE_BOXES[] = {
		BOX_VACCINE_1_DATE, BOX_VACCINE_2_DATE, BOX_VACCINE_3_DATE
	};

	for (int i = 0; i < 3; i++) {
		std::string name_str, date_str;
		
		if (!extract_field(name_str, binary, VAX_NAME_BOXES[i], tf, ctx))
			continue;  // No vaccine in this slot
		
		if (name_str.empty())
			continue;  // Empty name, skip
		
		out.vaccinations[i].name = name_str;
		
		// Try to get date, but don't fail if missing
		if (extract_field(date_str, binary, VAX_DATE_BOXES[i], tf, ctx)) {
			parse_date(out.vaccinations[i].expiration_date, date_str);
		}
	}

	return true;
}
