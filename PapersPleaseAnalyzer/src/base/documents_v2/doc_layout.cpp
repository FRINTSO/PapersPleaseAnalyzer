#include "pch.h"
#include "base/documents_v2/doc_layout.h"

namespace paplease {
	namespace documents {
		namespace v2 {

			const DocLayout& DocLayout::GetRef(AppearanceType type) noexcept
			{
				switch (type)
				{
					case AppearanceType::Invalid:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::Invalid);
						return layout;
					}
					case AppearanceType::AccessPermit:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::AccessPermit);
						return layout;
					}
					case AppearanceType::CertificateOfVaccination:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::CertificateOfVaccination);
						return layout;
					}
					case AppearanceType::DiplomaticAuthorization:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::DiplomaticAuthorization);
						return layout;
					}
					case AppearanceType::EntryPermit:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::EntryPermit);
						return layout;
					}
					case AppearanceType::EntryTicket:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::EntryTicket);
						return layout;
					}
					case AppearanceType::GrantOfAsylum:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::GrantOfAsylum);
						return layout;
					}
					case AppearanceType::IdentityCard:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::IdentityCard);
						return layout;
					}
					case AppearanceType::IdentitySupplement:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::IdentitySupplement);
						return layout;
					}
					case AppearanceType::WorkPass:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::WorkPass);
						return layout;
					}
					case AppearanceType::Passport_Antegria:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::Passport_Antegria);
						return layout;
					}
					case AppearanceType::Passport_Arstotzka:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::Passport_Arstotzka);
						return layout;
					}
					case AppearanceType::Passport_Impor:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::Passport_Impor);
						return layout;
					}
					case AppearanceType::Passport_Kolechia:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::Passport_Kolechia);
						return layout;
					}
					case AppearanceType::Passport_Obristan:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::Passport_Obristan);
						return layout;
					}
					case AppearanceType::Passport_Republia:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::Passport_Republia);
						return layout;
					}
					case AppearanceType::Passport_UnitedFederation:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::Passport_UnitedFederation);
						return layout;
					}
					case AppearanceType::RuleBook:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::RuleBook);
						return layout;
					}
					case AppearanceType::Bulletin:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::Bulletin);
						return layout;
					}
					case AppearanceType::Transcript:
					{
						static const auto layout = DocLayout::GetInstant(AppearanceType::Transcript);
						return layout;
					}
					default:
					{
						std::cerr << "Missing DocLayout::GetRef() Implementation\n";
						assert(false);
					}
				}
			}

		}  // namespace v2
	}  // namespace documents
}  // namespace paplease