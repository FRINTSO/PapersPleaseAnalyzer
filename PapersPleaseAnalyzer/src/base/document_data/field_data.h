#pragma once
#include <string>

#include "base/document_data/date.h"

namespace Documents::Data
{

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
		Male,
		Female,
	};

	enum class SIUnit
	{
		KG,
		CM,
	};

	struct SIUnitValue
	{
		int value;
		SIUnit unit;
	};

	struct Vaccine
	{
		Date date;
		std::string name;
	};


	struct StrList
	{
		std::vector<std::string> strs;
	};
}