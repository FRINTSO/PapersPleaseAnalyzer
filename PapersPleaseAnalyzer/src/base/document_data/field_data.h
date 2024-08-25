#pragma once
#include <string>


struct Name
{
	std::string firstName;
	std::string lastName;
};

enum class Country
{
	Invalid = 0,
	Antegria,
	Arstotzka,
	Impor,
	Kolechia,
	Obristan,
	Republia,
	UnitedFederation,
};

enum class Sex
{
	Invalid = 0,
	Male,
	Female,
};