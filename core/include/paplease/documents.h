#ifndef PAPLEASE_DOCUMENTS_H
#define PAPLEASE_DOCUMENTS_H

#include <opencv2/core/mat.hpp>

#include <paplease/colorspace.h>
#include <paplease/compiler.h>
#include <paplease/types.h>
#include <paplease/ocr.h>

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

struct doc_data {};

// === COMMON TYPES ===

struct date_t {
	u8 day;
	u8 month;
	u16 year;
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

bool parse_access_permit(access_permit_data &out, const doc &document);
bool parse_certificate_of_vaccination(certificate_of_vaccination_data &out,
				      const doc &document);
bool parse_diplomatic_authorization(diplomatic_authorization_data &out,
				    const doc &document);
bool parse_entry_permit(entry_permit_data &out, const doc &document);
bool parse_entry_ticket(entry_ticket_data &out, const doc &document);
bool parse_grant_of_asylum(grant_of_asylum_data &out, const doc &document);
bool parse_identity_card(identity_card_data &out, const doc &document);
bool parse_identity_supplement(identity_supplement_data &out,
			       const doc &document);
bool parse_work_pass(work_pass_data &out, const doc &document);
bool parse_passport(passport_data &out, const doc &document);

#include <paplease/documents.inl.h>
#endif // PAPLEASE_DOCUMENTS_H
