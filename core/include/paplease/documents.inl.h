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
	case doc_variant::rulebook:
		return doc_type::rulebook;
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

constexpr country country_of(doc_variant variant)
{
	switch (variant) {
	case doc_variant::passport_antegria:
		return country::antegria;
	case doc_variant::passport_arstotzka:
		return country::arstotzka;
	case doc_variant::passport_impor:
		return country::impor;
	case doc_variant::passport_kolechia:
		return country::kolechia;
	case doc_variant::passport_obristan:
		return country::obristan;
	case doc_variant::passport_republia:
		return country::republia;
	case doc_variant::passport_united_federation:
		return country::united_federation;
	default:
		unreachable();
	}
}

constexpr doc_variant variant_from_country(country c)
{
	switch (c) {
	case country::antegria:
		return doc_variant::passport_antegria;
	case country::arstotzka:
		return doc_variant::passport_arstotzka;
	case country::impor:
		return doc_variant::passport_impor;
	case country::kolechia:
		return doc_variant::passport_kolechia;
	case country::obristan:
		return doc_variant::passport_obristan;
	case country::republia:
		return doc_variant::passport_republia;
	case country::united_federation:
		return doc_variant::passport_united_federation;
	default:
		unreachable();
	}
}

constexpr doc_variant variant_from_type(doc_type t)
{
	switch (t) {
	case doc_type::access_permit:
		return doc_variant::access_permit;
	case doc_type::certificate_of_vaccination:
		return doc_variant::certificate_of_vaccination;
	case doc_type::diplomatic_authorization:
		return doc_variant::diplomatic_authorization;
	case doc_type::entry_permit:
		return doc_variant::entry_permit;
	case doc_type::entry_ticket:
		return doc_variant::entry_ticket;
	case doc_type::grant_of_asylum:
		return doc_variant::grant_of_asylum;
	case doc_type::identity_card:
		return doc_variant::identity_card;
	case doc_type::identity_supplement:
		return doc_variant::identity_supplement;
	case doc_type::work_pass:
		return doc_variant::work_pass;
	case doc_type::rulebook:
		return doc_variant::rulebook;
	case doc_type::bulletin:
		return doc_variant::bulletin;
	case doc_type::transcript:
		return doc_variant::transcript;
	case doc_type::passport:
		unreachable();
	}
}

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
	// passport_antegria
	{ { { 49, 77, 33 } }, 1, 260, 324 },
	// passport_arstotzka
	{ { { 59, 72, 59 } }, 1, 260, 324 },
	// passport_impor
	{ { { 102, 31, 9 } }, 1, 260, 324 },
	// passport_kolechia
	{ { { 85, 37, 63 } }, 1, 260, 324 },
	// passport_obristan
	{ { { 138, 12, 12 } }, 1, 260, 324 },
	// passport_republia
	{ { { 76, 42, 27 } }, 1, 260, 324 },
	// passport_united_federation
	{ { { 35, 30, 85 } }, 1, 260, 324 },
};

constexpr const doc_appearance &get_appearance(doc_variant v)
{
	return g_appearances[static_cast<size_t>(v)];
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
