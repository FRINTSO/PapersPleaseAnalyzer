#include "pch.h"
#include "paplease/analysis_v1/data/criminals.h"

namespace paplease {
	namespace analysis_v1 {
		namespace data {

			using namespace documents;

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
	}  // namespace analysis_v1
} // namespace paplease
