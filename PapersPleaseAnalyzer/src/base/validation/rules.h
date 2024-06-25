#pragma once


// Rules:															In Effect (day)
// Entrant must have a passport										since day 1
// Arstotzkan citizens only											only day 1
// All documents must be current									since day 2
// Foreigners require an entry ticket								only on day 3
// Foreigners require an entry permit								on days 4-26
// Arstotzkan citizens must have an ID card							since day 4
// Workers must have a work pass									since day 6
// No weapons or contraband											since day 7
// All Kolechians must be searched									only on day 7
// Diplomats require authorization									since day 8
// Foreigners require an ID supplement								days 13-26
// No entry from Impor												only on day 19
// Asylum seekers must have a grant									since day 21
// Confiscate Arstotzkan passports belonging to Altan residents		on days 24-27
// No entry from United Federation									only on day 25
// Entrant must have polio vaccine certificate						since day 26
// Foreigners require an access permit								since day 27
// Confiscate all Arstotzkan passports								since day 28
//
// Unlisted Rules:
// Wanted criminals must not enter									since day 14
// All denials must be accompanied by a reason for denial			since day 18
//

struct RuleDaySpecifier {
	int firstDay;
	int lastDay; // -1 = no end date
};

struct Rule {
	// some way to use effect
	RuleDaySpecifier inEffect;
	bool isActive;
};

