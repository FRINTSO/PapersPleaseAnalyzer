#pragma once
#include <array>
#include <string>

#include "base/documents_v2/doc_class.h"

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
public:
	Rule() = default;
private:
	std::string m_description;
	void* m_implication;
	void* m_inEffect;
	ERule m_rule;
};

class RuleBook
{
public:
	RuleBook() = default;

	friend RuleBook CreateRuleBook(const Documents::V2::Doc& document);
private:
	static constexpr size_t RuleCapacity = 10;
private:
	std::array<Rule, RuleBook::RuleCapacity> m_rules;
	size_t m_ruleCount;
};

RuleBook CreateRuleBook(const Documents::V2::Doc& document);