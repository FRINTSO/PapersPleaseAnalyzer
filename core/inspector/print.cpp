#include <cstdio>

#include <paplease/documents.h>

static constexpr int MAX_VACCINATIONS = 3;

// === DOCUMENT PRINTING ===

void print_passport(const passport_data &data)
{
	printf("  [PASSPORT]\n");
	printf("	name:		%s\n", data.name.c_str());
	printf("	number:		%s\n", data.passport_number.c_str());
	printf("	city:		%s\n", data.issuing_city.c_str());
	printf("	dob:		%02d.%02d.%d\n", data.date_of_birth.day,
	       data.date_of_birth.month, data.date_of_birth.year);
	printf("	expiration: %02d.%02d.%d\n", data.expiration.day,
	       data.expiration.month, data.expiration.year);
	printf("	sex:		%s\n", data.is_male ? "M" : "F");
}

void print_entry_permit(const entry_permit_data &data)
{
	printf("  [ENTRY PERMIT]\n");
	printf("	name:		%s\n", data.name.c_str());
	printf("	passport:	%s\n", data.passport_number.c_str());
	printf("	purpose:	%s\n", data.purpose.c_str());
	printf("	duration:	%s\n", data.duration.c_str());
	printf("	expiration: %02d.%02d.%d\n", data.expiration.day,
	       data.expiration.month, data.expiration.year);
}

void print_entry_ticket(const entry_ticket_data &data)
{
	printf("  [ENTRY TICKET]\n");
	printf("	valid_date: %02d.%02d.%d\n", data.valid_date.day,
	       data.valid_date.month, data.valid_date.year);
}

void print_work_pass(const work_pass_data &data)
{
	printf("  [WORK PASS]\n");
	printf("	name:	  %s\n", data.name.c_str());
	printf("	field:	  %s\n", data.work_field.c_str());
	printf("	end_date: %02d.%02d.%d\n", data.end_date.day,
	       data.end_date.month, data.end_date.year);
}

void print_access_permit(const access_permit_data &data)
{
	printf("  [ACCESS PERMIT]\n");
	printf("	name:		%s\n", data.name.c_str());
	printf("	passport:	%s\n", data.passport_number.c_str());
	printf("	country:	%s\n", data.issuing_country.c_str());
	printf("	purpose:	%s\n", data.purpose.c_str());
	printf("	duration:	%s\n", data.duration.c_str());
	printf("	height:		%dcm\n", data.height_cm);
	printf("	weight:		%dkg\n", data.weight_kg);
	printf("	physical:	%s\n", data.physical_desc.c_str());
	printf("	expiration: %02d.%02d.%d\n", data.expiration.day,
	       data.expiration.month, data.expiration.year);
}

void print_diplomatic_auth(const diplomatic_authorization_data &data)
{
	printf("  [DIPLOMATIC AUTH]\n");
	printf("	name:	  %s\n", data.name.c_str());
	printf("	passport: %s\n", data.passport_number.c_str());
	printf("	from:	  %s\n", data.issuing_country.c_str());
	printf("	access:   %s\n", data.access_countries.c_str());
}

void print_grant_of_asylum(const grant_of_asylum_data &data)
{
	printf("  [GRANT OF ASYLUM]\n");
	printf("	name:		%s\n", data.name.c_str());
	printf("	passport:	%s\n", data.passport_number.c_str());
	printf("	from:		%s\n", data.issuing_country.c_str());
	printf("	dob:		%02d.%02d.%d\n", data.date_of_birth.day,
	       data.date_of_birth.month, data.date_of_birth.year);
	printf("	height:		%dcm\n", data.height_cm);
	printf("	weight:		%dkg\n", data.weight_kg);
	printf("	expiration: %02d.%02d.%d\n", data.expiration.day,
	       data.expiration.month, data.expiration.year);
}

void print_identity_card(const identity_card_data &data)
{
	printf("  [ID CARD]\n");
	printf("	name:	  %s\n", data.name.c_str());
	printf("	district: %s\n", data.district.c_str());
	printf("	dob:	  %02d.%02d.%d\n", data.date_of_birth.day,
	       data.date_of_birth.month, data.date_of_birth.year);
	printf("	height:   %dcm\n", data.height_cm);
	printf("	weight:   %dkg\n", data.weight_kg);
}

void print_identity_supplement(const identity_supplement_data &data)
{
	printf("  [ID SUPPLEMENT]\n");
	printf("	height:		%dcm\n", data.height_cm);
	printf("	weight:		%dkg\n", data.weight_kg);
	printf("	physical:	%s\n", data.physical_desc.c_str());
	printf("	expiration: %02d.%02d.%d\n", data.expiration.day,
	       data.expiration.month, data.expiration.year);
}

void print_vaccination_cert(const certificate_of_vaccination_data &data)
{
	printf("  [VACCINATION CERT]\n");
	printf("	name:	  %s\n", data.name.c_str());
	printf("	passport: %s\n", data.passport_number.c_str());
	for (int i = 0; i < MAX_VACCINATIONS; i++) {
		if (!data.vaccinations[i].name.empty()) {
			printf("	vaccine%d: %s (%02d.%02d.%d)\n", i + 1,
			       data.vaccinations[i].name.c_str(),
			       data.vaccinations[i].expiration_date.day,
			       data.vaccinations[i].expiration_date.month,
			       data.vaccinations[i].expiration_date.year);
		}
	}
}
