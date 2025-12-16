#ifndef PAPLEASE_DOCUMENTS_H
#define PAPLEASE_DOCUMENTS_H

#include <opencv2/core/mat.hpp>

#include <paplease/colorspace.h>
#include <paplease/compiler.h>
#include <paplease/date.h>
#include <paplease/ocr.h>
#include <paplease/resources.h>
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

struct doc {
	cv::Mat pixels;
	doc_type type;
	country issuing_country; // Only meaningful for passport, but that's fine
};

struct doc_appearance {
	rgb_color border_colors[4];
	u8 color_count;
	u16 width;
	u16 height;
};

// === PER-DOCUMENT DATA STRUCTS ===

struct access_permit_data {
	std::string name;
	std::string passport_number;
	std::string issuing_country;
	std::string purpose;
	std::string duration;
	std::string physical_desc;
	u16 height_cm;
	u16 weight_kg;
	date_t expiration;
};

struct certificate_of_vaccination_data {
	struct vaccination_row {
		date_t expiration_date;
		std::string name;
	};
	std::string name;
	std::string passport_number;
	vaccination_row vaccinations[3]; // up to 3 vaccines
};

struct diplomatic_authorization_data {
	std::string name;
	std::string passport_number;
	std::string issuing_country;
	std::string access_countries; // comma-separated list
};

struct entry_permit_data {
	std::string name;
	std::string passport_number;
	std::string purpose;
	std::string duration;
	date_t expiration;
};

struct entry_ticket_data {
	date_t valid_date;
};

struct grant_of_asylum_data {
	std::string name;
	std::string passport_number;
	std::string issuing_country;
	date_t date_of_birth;
	date_t expiration;
	u16 height_cm;
	u16 weight_kg;
	// photo/fingerprints handled separately if needed
};

struct identity_card_data {
	std::string name;
	std::string district;
	date_t date_of_birth;
	u16 height_cm;
	u16 weight_kg;
	// photo handled separately
};

struct identity_supplement_data {
	std::string physical_desc;
	u16 height_cm;
	u16 weight_kg;
	date_t expiration;
	// thumbprint handled separately
};

struct work_pass_data {
	std::string name;
	std::string work_field;
	date_t end_date;
};

struct passport_data {
	std::string name;
	std::string passport_number;
	std::string issuing_city;
	date_t date_of_birth;
	date_t expiration;
	bool is_male;
	// photo handled separately
};

// === PARSE FUNCTIONS ===

bool parse_access_permit(access_permit_data &out, const doc &document,
			 const resources_ctx &ctx);
bool parse_certificate_of_vaccination(certificate_of_vaccination_data &out,
				      const doc &document,
				      const resources_ctx &ctx);
bool parse_diplomatic_authorization(diplomatic_authorization_data &out,
				    const doc &document,
				    const resources_ctx &ctx);
bool parse_entry_permit(entry_permit_data &out, const doc &document,
			const resources_ctx &ctx);
bool parse_entry_ticket(entry_ticket_data &out, const doc &document,
			const resources_ctx &ctx);
bool parse_grant_of_asylum(grant_of_asylum_data &out, const doc &document,
			   const resources_ctx &ctx);
bool parse_identity_card(identity_card_data &out, const doc &document,
			 const resources_ctx &ctx);
bool parse_identity_supplement(identity_supplement_data &out,
			       const doc &document, const resources_ctx &ctx);
bool parse_work_pass(work_pass_data &out, const doc &document,
		     const resources_ctx &ctx);
bool parse_passport(passport_data &out, const doc &document,
		    const resources_ctx &ctx);

inline constexpr doc_appearance g_appearances[] = {
	// access_permit
	{ { { 215, 233, 210 }, { 176, 168, 173 } }, 2, 294, 392 },
	// certificate_of_vaccination
	{ { { 234, 223, 128 }, { 177, 151, 97 } }, 2, 270, 312 },
	// diplomatic_authorization
	{ { { 238, 238, 251 }, { 190, 202, 209 }, { 154, 179, 168 } },
	  3,
	  300,
	  400 },
	// entry_permit
	{ { { 224, 233, 199 }, { 202, 201, 175 } }, 2, 300, 402 },
	// entry_ticket
	{ { { 224, 233, 199 }, { 180, 169, 151 } }, 2, 280, 102 },
	// grant_of_asylum
	{ { { 253, 222, 223 }, { 180, 157, 175 } }, 2, 320, 374 },
	// identity_card
	{ { { 217, 189, 247 }, { 178, 156, 204 } }, 2, 252, 142 },
	// identity_supplement
	{ { { 232, 218, 199 }, { 181, 167, 166 } }, 2, 180, 300 },
	// work_pass
	{ { { 233, 199, 211 }, { 204, 175, 192 } }, 2, 294, 270 },
	// rulebook
	{ { { 87, 72, 72 } }, 1, 480, 320 },
	// bulletin
	{ { { 240, 240, 240 } }, 1, 298, 398 },
	// transcript
	{ { { 210, 237, 236 } }, 1, 300, 400 },
	// passport
	{ {}, 0, 260, 324 },  // Placeholder - dimensions only
};

// Passport-specific appearances - one per country
inline constexpr doc_appearance g_passport_appearances[] = {
	/* antegria */		  { { { 49, 77, 33 } }, 1, 260, 324 },
	/* arstotzka */		  { { { 59, 72, 59 } }, 1, 260, 324 },
	/* impor */		  { { { 102, 31, 9 } }, 1, 260, 324 },
	/* kolechia */		  { { { 85, 37, 63 } }, 1, 260, 324 },
	/* obristan */		  { { { 138, 12, 12 } }, 1, 260, 324 },
	/* republia */		  { { { 76, 42, 27 } }, 1, 260, 324 },
	/* united_federation */   { { { 35, 30, 85 } }, 1, 260, 324 },
};

constexpr const doc_appearance &get_appearance(doc_type t)
{
	return g_appearances[static_cast<size_t>(t)];
}

constexpr const doc_appearance& get_passport_appearance(country c) {
	return g_passport_appearances[static_cast<size_t>(c)];
}

constexpr typeface typeface_for(doc_type type)
{
	switch (type) {
	case doc_type::access_permit:
	case doc_type::certificate_of_vaccination:
	case doc_type::diplomatic_authorization:
	case doc_type::entry_permit:
	case doc_type::entry_ticket:
	case doc_type::grant_of_asylum:
	case doc_type::identity_supplement:
	case doc_type::work_pass:
	case doc_type::rulebook:
	case doc_type::bulletin:
	case doc_type::passport:
		return typeface::bm_mini;
	case doc_type::identity_card:
		return typeface::mini_kylie;
	case doc_type::transcript:
		return typeface::t04b03;
	}
	unreachable();
}
#endif // PAPLEASE_DOCUMENTS_H
