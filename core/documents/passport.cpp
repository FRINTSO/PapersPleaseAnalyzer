#include <cassert>
#include <paplease/documents.h>
#include <paplease/geometry.h>
#include <paplease/ocr.h>
#include "parse_helpers.h"

struct passport_layout {
	rectangle name;
	rectangle passport_number;
	rectangle issuing_city;
	rectangle date_of_birth;
	rectangle expiration;
	rectangle sex;
};

static constexpr passport_layout LAYOUT_ANTEGRIA = {
	{16, 278, 230, 16}, {16, 298, 230, 16}, {50, 238, 116, 16},
	{50, 202, 116, 16}, {50, 256, 116, 16}, {50, 220, 116, 16},
};

static constexpr passport_layout LAYOUT_ARSTOTZKA = {
	{16, 176, 230, 16}, {16, 296, 124, 16}, {136, 228, 116, 16},
	{136, 196, 116, 16}, {136, 244, 116, 16}, {136, 212, 116, 16},
};

static constexpr passport_layout LAYOUT_IMPOR = {
	{16, 172, 230, 16}, {128, 292, 118, 16}, {140, 226, 116, 16},
	{140, 194, 116, 16}, {140, 242, 116, 16}, {140, 210, 116, 16},
};

static constexpr passport_layout LAYOUT_KOLECHIA = {
	{16, 196, 230, 16}, {130, 296, 118, 16}, {138, 246, 116, 16},
	{138, 214, 116, 16}, {136, 262, 116, 16}, {138, 230, 116, 16},
};

static constexpr passport_layout LAYOUT_OBRISTAN = {
	{16, 196, 230, 16}, {20, 296, 118, 16}, {54, 254, 116, 16},
	{54, 222, 116, 16}, {54, 270, 116, 16}, {54, 238, 116, 16},
};

static constexpr passport_layout LAYOUT_REPUBLIA = {
	{16, 174, 230, 16}, {130, 296, 118, 16}, {54, 228, 116, 16},
	{54, 196, 116, 16}, {54, 244, 116, 16}, {54, 212, 116, 16},
};

static constexpr passport_layout LAYOUT_UNITED_FED = {
	{16, 196, 230, 16}, {130, 296, 118, 16}, {138, 244, 116, 16},
	{138, 212, 116, 16}, {138, 260, 116, 16}, {138, 228, 116, 16},
};

static const passport_layout *get_passport_layout(country c)
{
	switch (c) {
	case country::antegria:          return &LAYOUT_ANTEGRIA;
	case country::arstotzka:         return &LAYOUT_ARSTOTZKA;
	case country::impor:             return &LAYOUT_IMPOR;
	case country::kolechia:          return &LAYOUT_KOLECHIA;
	case country::obristan:          return &LAYOUT_OBRISTAN;
	case country::republia:          return &LAYOUT_REPUBLIA;
	case country::united_federation: return &LAYOUT_UNITED_FED;
	default: return nullptr;
	}
}

bool parse_passport(passport_data &out, const doc &document,
		    const resources_ctx &ctx)
{
	assert(document.type == doc_type::passport);
	const passport_layout *layout = get_passport_layout(document.issuing_country);
	if (!layout)
		return false;

	cv::Mat binary = preprocess_document(document.pixels);
	typeface tf = typeface_for(doc_type::passport);

	std::string tmp;

	if (!extract_field(out.name, binary, layout->name, tf, ctx))
		return false;

	if (!extract_field(out.passport_number, binary, layout->passport_number,
			   tf, ctx))
		return false;

	if (!extract_field(out.issuing_city, binary, layout->issuing_city, tf,
			   ctx))
		return false;

	if (!extract_field(tmp, binary, layout->date_of_birth, tf, ctx))
		return false;
	if (!parse_date(out.date_of_birth, tmp))
		return false;

	if (!extract_field(tmp, binary, layout->expiration, tf, ctx))
		return false;
	if (!parse_date(out.expiration, tmp))
		return false;

	if (!extract_field(tmp, binary, layout->sex, tf, ctx))
		return false;
	out.is_male = (tmp.size() > 0 && tmp[0] == 'M');

	return true;
}
