#pragma once

namespace paplease {
	namespace documents {

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
