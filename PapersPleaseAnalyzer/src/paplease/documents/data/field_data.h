#pragma once
#include "paplease/documents/data/date.h"

#include <string>
#include <vector>

namespace paplease {
	namespace documents {
		namespace data {

			enum class Sex : u8
			{
				Male,
				Female,
			};

			enum class SIUnit : u8
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
				std::string name;
				Date date;

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
