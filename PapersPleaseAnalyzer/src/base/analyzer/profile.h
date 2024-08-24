#pragma once
#include <string>
#include "base/documents_v2/data/date.h"

// A profile on the current applicant

struct {
	// Height
	// Weight
	// Birthdate
	// Sex
	// Appearance
	// Finger print
	// Documents
	// Name
	// Document Id
	// 
	// 
	//
};

struct Name {
	std::string firstName;
	std::string lastName;
};

enum class Country {
	Invalid = 0,
	Antegria,
	Arstotzka,
	Impor,
	Kolechia,
	Obristan,
	Republia,
	UnitedFederation,
};

enum class Sex {
	Invalid = 0,
	Male,
	Female,
};


class Profile {
private:
	Name m_name;
	std::string m_idNumber;
	Date m_dateOfBirth;
	Country m_nationality;
	int m_height;
	int m_weight;
	Sex m_sex;
};
