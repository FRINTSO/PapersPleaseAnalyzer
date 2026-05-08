#include <paplease/observation.h>

#include <string_view>

#include <paplease/documents.h>
#include <paplease/inspector.h>
#include <paplease/vision.h>

#include "print.h"

observation observe_frame(const game_screen &screen, const resources_ctx &ctx,
			  const observe_options &opts)
{
	observation obs;

	// === BOOTH ===
	booth_info booth;
	obs.booth_ok = extract_booth_info(booth, screen, ctx);
	if (obs.booth_ok) {
		obs.date = booth.current_date;
		obs.entrant_count = std::stoi(booth.entrant_count);
		obs.entrant_weight = booth.entrant_weight;
	}

	// === RULEBOOK ===
	if (!opts.skip_rulebook) {
		doc rulebook;
		obs.rulebook_visible =
			find_document(rulebook, doc_type::rulebook,
				      ui_section::inspection, screen);
		if (obs.rulebook_visible) {
			obs.rulebook_ok = parse_rules(obs.rules, rulebook, ctx);
		}
	}

	// === ENTRANT DOCUMENTS ===
	auto detected = scan_for_documents(screen, ui_section::inspection);

	for (const auto &d : detected) {
		// skip non-entrant docs
		if (d.type == doc_type::rulebook ||
		    d.type == doc_type::bulletin ||
		    d.type == doc_type::transcript)
			continue;

		obs.entrant_docs.visible.push_back(d.type);
		auto &f = obs.entrant_docs.identity_fields[d.type];

		switch (d.type) {
		case doc_type::passport: { //✅
			passport_data data;
			if (parse_passport(data, d, ctx)) {
				f[fact_field::name] = data.name;
				f[fact_field::passport_number] = data.passport_number;
				f[fact_field::issuing_city] = data.issuing_city;
				f[fact_field::date_of_birth] = format_date(data.date_of_birth);
				f[fact_field::sex] = data.is_male ? "M" : "F";

				std::string_view country_name;
				switch (d.issuing_country) {
				case country::antegria:
					country_name = "ANTEGRIA";
					break;
				case country::arstotzka:
					country_name = "ARSTOTZKA";
					break;
				case country::impor:
					country_name = "IMPOR";
					break;
				case country::kolechia:
					country_name = "KOLECHIA";
					break;
				case country::obristan:
					country_name = "OBRISTAN";
					break;
				case country::republia:
					country_name = "REPUBLIA";
					break;
				case country::united_federation:
					country_name = "UNITEDFED";
					break;
				}
				f[fact_field::nationality] = country_name;

				obs.entrant_docs.expirations[d.type] = data.expiration;

				// cannot be invalid, it determines source of truth of nationality
				obs.entrant_docs.nationality = d.issuing_country;

				print_passport(data);
			}
			break;
		}

		case doc_type::entry_permit: {
			entry_permit_data data;
			if (parse_entry_permit(data, d, ctx)) {
				f[fact_field::name] = data.name;
				f[fact_field::passport_number] = data.passport_number;
				f[fact_field::purpose] = data.purpose;
				f[fact_field::duration] = data.duration;

				// Typed field -> separate map

				obs.entrant_docs.expirations[d.type] = data.expiration;

				print_entry_permit(data);
			}
			break;
		}

			case doc_type::entry_ticket: { //✅
			entry_ticket_data data;
			if (parse_entry_ticket(data, d, ctx)) {
				f[fact_field::valid_date] = format_date(data.valid_date);

				print_entry_ticket(data);
			}
			break;
		}

		case doc_type::work_pass: { //✅
			work_pass_data data;
			if (parse_work_pass(data, d, ctx)) {
				f[fact_field::name] = data.name;
				f[fact_field::work_field] = data.work_field;
				f[fact_field::end_date] = format_date(data.end_date);

				print_work_pass(data);

				// TODO: Check field
			}
			break;
		}

		case doc_type::access_permit: { //✅
			access_permit_data data;
			if (parse_access_permit(data, d, ctx)) {
				f[fact_field::name] = data.name;
				f[fact_field::passport_number] = data.passport_number;
				f[fact_field::nationality] = data.issuing_country;
				f[fact_field::purpose] = data.purpose;
				f[fact_field::duration] = data.duration;
				f[fact_field::height_cm] = std::to_string(data.height_cm);
				f[fact_field::weight_kg] = std::to_string(data.weight_kg);
				f[fact_field::physical_desc] = data.physical_desc;

				obs.entrant_docs.expirations[d.type] = data.expiration;

				print_access_permit(data);

				// TODO: Check seal
			}
			break;
		}

		case doc_type::identity_card: { //✅
			identity_card_data data;
			if (parse_identity_card(data, d, ctx)) {
				f[fact_field::name] = data.name;
				f[fact_field::district] = data.district;
				f[fact_field::date_of_birth] = format_date(data.date_of_birth);
				f[fact_field::height_cm] = std::to_string(data.height_cm);
				f[fact_field::weight_kg] = std::to_string(data.weight_kg);

				print_identity_card(data);
			}
			break;
		}

		case doc_type::grant_of_asylum: { //✅
			grant_of_asylum_data data;
			if (parse_grant_of_asylum(data, d, ctx)) {
				f[fact_field::name] = data.name;
				f[fact_field::passport_number] = data.passport_number;
				f[fact_field::nationality] = data.issuing_country;
				f[fact_field::date_of_birth] = format_date(data.date_of_birth);
				f[fact_field::height_cm] = std::to_string(data.height_cm);
				f[fact_field::weight_kg] = std::to_string(data.weight_kg);

				obs.entrant_docs.expirations[d.type] = data.expiration;
				// TODO: Check seal

				print_grant_of_asylum(data);
			}
			break;
		}

		case doc_type::diplomatic_authorization: {
			diplomatic_authorization_data data;
			if (parse_diplomatic_authorization(data, d, ctx)) {
				f[fact_field::name] = data.name;
				f[fact_field::passport_number] = data.passport_number;
				f[fact_field::access_countries] = data.access_countries;

				print_diplomatic_auth(data);
			}
			break;
		}

		case doc_type::identity_supplement: {
			identity_supplement_data data;
			if (parse_identity_supplement(data, d, ctx)) {
				f[fact_field::height_cm] = std::to_string(data.height_cm);
				f[fact_field::weight_kg] = std::to_string(data.weight_kg);
				f[fact_field::physical_desc] = data.physical_desc;

				obs.entrant_docs.expirations[d.type] = data.expiration;

				print_identity_supplement(data);
			}
			break;
		}

		case doc_type::certificate_of_vaccination: { //✅
			certificate_of_vaccination_data data;
			if (parse_certificate_of_vaccination(data, d, ctx)) {
				f[fact_field::name] = data.name;
				f[fact_field::passport_number] = data.passport_number;
				// store vaccines if needed
				constexpr size_t MAX_VACCINES =
					sizeof(data.vaccinations) /
					sizeof(data.vaccinations[0]);
				for (int i = 0; i < MAX_VACCINES; i++) {
					obs.entrant_docs.vaccines[i] =
						data.vaccinations[i];
				}

				print_vaccination_cert(data);
			}
			break;
		}

		default:
			break;
		}
	}

	return obs;
}
