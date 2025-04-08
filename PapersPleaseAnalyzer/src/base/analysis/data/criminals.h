#pragma once
#include <array>

#include "base/documents/doc_class.h"
#include "base/documents/data/photo.h"

namespace paplease {
	namespace analysis {
		namespace data {

			class CriminalData
			{
			public:
				CriminalData() = default;

				friend CriminalData CreateCriminalData(const documents::Doc& document);
			private:
				static constexpr size_t CriminalCapacity = 3;
			private:
				std::array<documents::data::Photo, CriminalData::CriminalCapacity> m_photos;
			};

			CriminalData CreateCriminalData(const documents::Doc& document);

		}  // namespace data
	}  // namespace analysis
}  // namespace paplease
