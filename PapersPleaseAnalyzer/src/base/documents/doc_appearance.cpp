#include "pch.h"
#include "base/documents/doc_appearance.h"

namespace paplease {
	namespace documents {

		const DocAppearance& DocAppearance::GetRef(AppearanceType type) noexcept
		{
			switch (type)
			{
				case AppearanceType::Invalid:
				{
					static constinit const auto appearance = DocAppearance::GetInstant(AppearanceType::Invalid);
					return appearance;
				}
				case AppearanceType::AccessPermit:
				{
					static constinit const auto appearance = DocAppearance::GetInstant(AppearanceType::AccessPermit);
					return appearance;
				}
				case AppearanceType::CertificateOfVaccination:
				{
					static constinit const auto appearance = DocAppearance::GetInstant(AppearanceType::CertificateOfVaccination);
					return appearance;
				}
				case AppearanceType::DiplomaticAuthorization:
				{
					static constinit const auto appearance = DocAppearance::GetInstant(AppearanceType::DiplomaticAuthorization);
					return appearance;
				}
				case AppearanceType::EntryPermit:
				{
					static constinit const auto appearance = DocAppearance::GetInstant(AppearanceType::EntryPermit);
					return appearance;
				}
				case AppearanceType::EntryTicket:
				{
					static constinit const auto appearance = DocAppearance::GetInstant(AppearanceType::EntryTicket);
					return appearance;
				}
				case AppearanceType::GrantOfAsylum:
				{
					static constinit const auto appearance = DocAppearance::GetInstant(AppearanceType::GrantOfAsylum);
					return appearance;
				}
				case AppearanceType::IdentityCard:
				{
					static constinit const auto appearance = DocAppearance::GetInstant(AppearanceType::IdentityCard);
					return appearance;
				}
				case AppearanceType::IdentitySupplement:
				{
					static constinit const auto appearance = DocAppearance::GetInstant(AppearanceType::IdentitySupplement);
					return appearance;
				}
				case AppearanceType::WorkPass:
				{
					static constinit const auto appearance = DocAppearance::GetInstant(AppearanceType::WorkPass);
					return appearance;
				}
				case AppearanceType::Passport_Antegria:
				{
					static constinit const auto appearance = DocAppearance::GetInstant(AppearanceType::Passport_Antegria);
					return appearance;
				}
				case AppearanceType::Passport_Arstotzka:
				{
					static constinit const auto appearance = DocAppearance::GetInstant(AppearanceType::Passport_Arstotzka);
					return appearance;
				}
				case AppearanceType::Passport_Impor:
				{
					static constinit const auto appearance = DocAppearance::GetInstant(AppearanceType::Passport_Impor);
					return appearance;
				}
				case AppearanceType::Passport_Kolechia:
				{
					static constinit const auto appearance = DocAppearance::GetInstant(AppearanceType::Passport_Kolechia);
					return appearance;
				}
				case AppearanceType::Passport_Obristan:
				{
					static constinit const auto appearance = DocAppearance::GetInstant(AppearanceType::Passport_Obristan);
					return appearance;
				}
				case AppearanceType::Passport_Republia:
				{
					static constinit const auto appearance = DocAppearance::GetInstant(AppearanceType::Passport_Republia);
					return appearance;
				}
				case AppearanceType::Passport_UnitedFederation:
				{
					static constinit const auto appearance = DocAppearance::GetInstant(AppearanceType::Passport_UnitedFederation);
					return appearance;
				}
				case AppearanceType::RuleBook:
				{
					static constinit const auto layout = DocAppearance::GetInstant(AppearanceType::RuleBook);
					return layout;
				}
				case AppearanceType::Bulletin:
				{
					static constinit const auto layout = DocAppearance::GetInstant(AppearanceType::Bulletin);
					return layout;
				}
				case AppearanceType::Transcript:
				{
					static constinit const auto layout = DocAppearance::GetInstant(AppearanceType::Transcript);
					return layout;
				}
				default:
				{
					std::cerr << "Missing DocAppearance::GetRef() Implementation\n";
					assert(false);
				}
			}
		}

	}  // namespace documents
}  // namespace paplease
