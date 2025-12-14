#ifndef PAPLEASE_DOCUMENTS_H
#define PAPLEASE_DOCUMENTS_H

#include <opencv2/core/mat.hpp>

#include <paplease/colorspace.h>
#include <paplease/compiler.h>
#include <paplease/types.h>

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
	rulebook,
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
	rulebook,
	bulletin,
	transcript,
	passport_antegria,
	passport_arstotzka,
	passport_impor,
	passport_kolechia,
	passport_obristan,
	passport_republia,
	passport_united_federation
};

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

struct doc_data {
};

#include <paplease/documents.inl.h>
#endif // PAPLEASE_DOCUMENTS_H
