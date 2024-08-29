#pragma once
#include <array>
#include <string>

enum class ERule
{
	Invalid = 0,
	RequirePassport,
	RequireArstotzkaPassport,
	RequireCurrentDocuments,
	RequireEntryTicketFromForeigners,
	RequireIdentityCardFromCitizens,
	RequireWorkPassFromWorkers,
	ProhibitWeaponsAndContraband,
	RequireSearchOfKolechians,
	RequireDiplomaticAuthorizationFromDiplomats,
	RequireIdentitySupplementFromForeigners,
	ProhibitEntryFromImpor,
	RequireGrantFromAsylumSeekers,
	ConfiscateArstotzkanPassportsFromAltanRegion,
	ProhibitEntryFromUnitedFederation,
	RequirePolioVaccination,
	RequireAccessPermitFromForeigners,
	ConfiscateArstotzkanPassports,
};

class Rule
{
private:
	std::string m_description;
	void* m_implication;
	void* m_inEffect;
	ERule m_rule;
};

class RuleBook
{
private:
	std::array<Rule, 10> m_rules;
};