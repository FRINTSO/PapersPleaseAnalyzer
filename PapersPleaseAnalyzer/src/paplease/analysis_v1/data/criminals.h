#pragma once
#include <array>

#include "paplease/documents/doc_class.h"
#include "paplease/documents/data/photo.h"

namespace paplease {
	namespace analysis_v1 {
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
	}  // namespace analysis_v1
}  // namespace paplease
