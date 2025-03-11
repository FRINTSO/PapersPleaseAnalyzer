#include "pch.h"
#include "base/game_state/data/criminals.h"

namespace paplease {
	namespace analysis {
		namespace data {

			using namespace documents::v2;

			CriminalData CreateCriminalData(const Doc& document)
			{
				if (!document.IsValid())
				{
					std::cerr << "Tried creating criminal data from invalid document\n";
					return {};
				}

				return {};
			}

		}  // namespace data
	}  // namespace analysis
} // namespace paplease
