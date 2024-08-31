#pragma once
#include <array>

namespace Documents::V2
{
	enum class DocType
	{
		Invalid = 0,
		AccessPermit,
		CertificateOfVaccination,
		DiplomaticAuthorization,
		EntryPermit,
		EntryTicket,
		GrantOfAsylum,
		IdentityCard,
		IdentitySupplement,
		WorkPass,
		RuleBook,
		Bulletin,
		Transcript = 12,
		Passport,
	};

	enum class PassportType
	{
		Invalid = 0,
		Antegria = DocType::Passport,
		Arstotzka,
		Impor,
		Kolechia,
		Obristan,
		Republia,
		UnitedFederation,
	};


#if USE_ENUM_FUNCS
	static constexpr bool IsApplicantDocument(DocType documentType)
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

	static consteval std::array<DocType, static_cast<size_t>(DocType::Passport)> GetDocTypeIterator()
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
#endif

}