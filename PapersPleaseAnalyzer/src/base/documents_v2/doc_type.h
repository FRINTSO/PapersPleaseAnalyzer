#pragma once

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
		WorkPass = 9,
		Passport,
	};

	enum class PassportType
	{
		Invalid = 0,
		Antegria = 10,
		Arstotzka,
		Impor,
		Kolechia,
		Obristan,
		Republia,
		UnitedFederation,
	};

}