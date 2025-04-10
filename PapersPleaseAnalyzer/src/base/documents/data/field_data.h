#pragma once
#include "base/documents/data/date.h"

#include <string>
#include <vector>

namespace paplease {
	namespace documents {
		namespace data {

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

				bool operator==(const SIUnitValue&) const = default;
			};

			struct Vaccine
			{
				Date date;
				std::string name;

				bool operator==(const Vaccine&) const = default;
			};


			struct StrList
			{
				std::vector<std::string> strs;

				bool operator==(const StrList&) const = default;
			};

		}  // namespace data
	}  // namespace documents
}  // namespace paplease
