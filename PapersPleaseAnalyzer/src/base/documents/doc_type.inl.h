#pragma once

namespace paplease {
	namespace documents {

		static std::string_view doc_type_string_views[] DOC_TYPE_NAMES;

		constexpr std::string_view& ToStringView(DocType documentType)
		{
			return doc_type_string_views[static_cast<int>(documentType)];
		}

		constexpr bool IsApplicantDocument(DocType documentType)
		{

            switch (documentType)
            {
                case DocType::AccessPermit:
                case DocType::CertificateOfVaccination:
                case DocType::DiplomaticAuthorization:
                case DocType::EntryPermit:
                case DocType::EntryTicket:
                case DocType::GrantOfAsylum:
                case DocType::IdentityCard:
                case DocType::IdentitySupplement:
                case DocType::WorkPass:
                case DocType::Passport:
                    return true;
                default:
                    return false;
            }
		}

        consteval std::array<DocType, static_cast<size_t>(DocType::Passport)> GetDocTypeIterator()
        {
            return std::array<DocType, static_cast<size_t>(DocType::Passport)>{
                DocType::AccessPermit,
                    DocType::CertificateOfVaccination,
                    DocType::DiplomaticAuthorization,
                    DocType::EntryPermit,
                    DocType::EntryTicket,
                    DocType::GrantOfAsylum,
                    DocType::IdentityCard,
                    DocType::IdentitySupplement,
                    DocType::WorkPass,
                    DocType::RuleBook,
                    DocType::Bulletin,
                    DocType::Transcript,
                    DocType::Passport,
            };
        }

    }  // namespace documents
}  // namespace paplease
