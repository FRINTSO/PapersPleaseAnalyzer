#pragma once
#include <string>
#include "base/document_data/date.h"
#include "base/document_data/field_data.h"

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
