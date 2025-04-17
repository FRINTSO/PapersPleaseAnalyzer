#pragma once

namespace paplease {
	namespace documents {

		static std::string_view doc_type_string_views[] DOC_TYPE_NAMES;

		constexpr const std::string_view& ToStringView(DocType documentType)
		{
            if (static_cast<size_t>(documentType) >= static_cast<size_t>(DocType::Count))
            {
                return doc_type_string_views[static_cast<int>(DocType::Count)];  // At this position is the INVALID string
            }
			return doc_type_string_views[static_cast<size_t>(documentType)];
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

        // Applicant documents and transcript
        constexpr bool IsApplicantRelatedDocument(DocType documentType)
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
                case DocType::Transcript:
                case DocType::WorkPass:
                case DocType::Passport:
                    return true;
                default:
                    return false;
            }
        }

    }  // namespace documents
}  // namespace paplease
