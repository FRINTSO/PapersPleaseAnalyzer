#ifndef PAPLEASE_DOCUMENTS_H
#define PAPLEASE_DOCUMENTS_H

#include "opencv2/core/mat.hpp"
#include "paplease/colors.h"
#include <paplease/compiler.h>
#include <paplease/integral_types.h>

enum class doc_type : u8 {
	access_permit = 0,
	certificate_of_vaccination,
	diplomatic_authorization,
	entry_permit,
	entry_ticket,
	grant_of_asylum,
	identity_card,
	identity_supplement,
	work_pass,
	rule_book,
	bulletin,
	transcript,
	passport,
};

enum class country : u8 {
	antegria = 0,
	arstotzka,
	impor,
	kolechia,
	obristan,
	republia,
	united_federation,
};

enum class doc_variant : u8 {
	access_permit = 0,
	certificate_of_vaccination,
	diplomatic_authorization,
	entry_permit,
	entry_ticket,
	grant_of_asylum,
	identity_card,
	identity_supplement,
	work_pass,
	rule_book,
	bulletin,
	transcript,
	passport_antegria,
	passport_arstotzka,
	passport_impor,
	passport_kolechia,
	passport_obristan,
	passport_republia,
	passport_united_federation,
};

constexpr doc_type type_of(doc_variant v)
{
	switch (v) {
	case doc_variant::access_permit:
		return doc_type::access_permit;
	case doc_variant::certificate_of_vaccination:
		return doc_type::certificate_of_vaccination;
	case doc_variant::diplomatic_authorization:
		return doc_type::diplomatic_authorization;
	case doc_variant::entry_permit:
		return doc_type::entry_permit;
	case doc_variant::entry_ticket:
		return doc_type::entry_ticket;
	case doc_variant::grant_of_asylum:
		return doc_type::grant_of_asylum;
	case doc_variant::identity_card:
		return doc_type::identity_card;
	case doc_variant::identity_supplement:
		return doc_type::identity_supplement;
	case doc_variant::work_pass:
		return doc_type::work_pass;
	case doc_variant::rule_book:
		return doc_type::rule_book;
	case doc_variant::bulletin:
		return doc_type::bulletin;
	case doc_variant::transcript:
		return doc_type::transcript;
	case doc_variant::passport_antegria:
	case doc_variant::passport_arstotzka:
	case doc_variant::passport_impor:
	case doc_variant::passport_kolechia:
	case doc_variant::passport_obristan:
	case doc_variant::passport_republia:
	case doc_variant::passport_united_federation:
		return doc_type::passport;
	}
	unreachable();
}

struct doc {
	cv::Mat pixels;
	doc_variant variant;
};

struct doc_appearance {
	rgb_color border_colors[4];
	u8 color_count;
	u16 width;
	u16 height;
};

constexpr doc_appearance get_appearance(doc_variant v);

#include <paplease/documents.inl.h>
#endif // PAPLEASE_DOCUMENTS_H
