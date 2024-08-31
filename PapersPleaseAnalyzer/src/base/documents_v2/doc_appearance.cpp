#include "pch.h"
#include "base/documents_v2/doc_appearance.h"

namespace Documents::V2 {

	const DocAppearance& DocAppearance::GetRef(AppearanceType type) noexcept
	{
		switch (type)
		{
			case AppearanceType::Invalid:
			{
				static const auto appearance = DocAppearance::GetInstant(AppearanceType::Invalid);
				return appearance;
			}
			case AppearanceType::AccessPermit:
			{
				static const auto appearance = DocAppearance::GetInstant(AppearanceType::Invalid);
				return appearance;
			}
			case AppearanceType::CertificateOfVaccination:
			{
				static const auto appearance = DocAppearance::GetInstant(AppearanceType::CertificateOfVaccination);
				return appearance;
			}
			case AppearanceType::DiplomaticAuthorization:
			{
				static const auto appearance = DocAppearance::GetInstant(AppearanceType::DiplomaticAuthorization);
				return appearance;
			}
			case AppearanceType::EntryPermit:
			{
				static const auto appearance = DocAppearance::GetInstant(AppearanceType::EntryPermit);
				return appearance;
			}
			case AppearanceType::EntryTicket:
			{
				static const auto appearance = DocAppearance::GetInstant(AppearanceType::EntryTicket);
				return appearance;
			}
			case AppearanceType::GrantOfAsylum:
			{
				static const auto appearance = DocAppearance::GetInstant(AppearanceType::GrantOfAsylum);
				return appearance;
			}
			case AppearanceType::IdentityCard:
			{
				static const auto appearance = DocAppearance::GetInstant(AppearanceType::IdentityCard);
				return appearance;
			}
			case AppearanceType::IdentitySupplement:
			{
				static const auto appearance = DocAppearance::GetInstant(AppearanceType::IdentitySupplement);
				return appearance;
			}
			case AppearanceType::WorkPass:
			{
				static const auto appearance = DocAppearance::GetInstant(AppearanceType::WorkPass);
				return appearance;
			}
			case AppearanceType::Passport_Antegria:
			{
				static const auto appearance = DocAppearance::GetInstant(AppearanceType::Passport_Antegria);
				return appearance;
			}
			case AppearanceType::Passport_Arstotzka:
			{
				static const auto appearance = DocAppearance::GetInstant(AppearanceType::Passport_Arstotzka);
				return appearance;
			}
			case AppearanceType::Passport_Impor:
			{
				static const auto appearance = DocAppearance::GetInstant(AppearanceType::Passport_Impor);
				return appearance;
			}
			case AppearanceType::Passport_Kolechia:
			{
				static const auto appearance = DocAppearance::GetInstant(AppearanceType::Passport_Kolechia);
				return appearance;
			}
			case AppearanceType::Passport_Obristan:
			{
				static const auto appearance = DocAppearance::GetInstant(AppearanceType::Passport_Obristan);
				return appearance;
			}
			case AppearanceType::Passport_Republia:
			{
				static const auto appearance = DocAppearance::GetInstant(AppearanceType::Passport_Republia);
				return appearance;
			}
			case AppearanceType::Passport_UnitedFederation:
			{
				static const auto appearance = DocAppearance::GetInstant(AppearanceType::Passport_UnitedFederation);
				return appearance;
			}
			case AppearanceType::RuleBook:
			{
				static const auto layout = DocAppearance::GetInstant(AppearanceType::RuleBook);
				return layout;
			}
			case AppearanceType::Bulletin:
			{
				static const auto layout = DocAppearance::GetInstant(AppearanceType::RuleBook);
				return layout;
			}
			case AppearanceType::Transcript:
			{
				static const auto layout = DocAppearance::GetInstant(AppearanceType::Transcript);
				return layout;
			}
			default:
			{
				std::cerr << "Missing DocAppearance::GetRef() Implementation\n";
				assert(false);
			}
		}
	}

}