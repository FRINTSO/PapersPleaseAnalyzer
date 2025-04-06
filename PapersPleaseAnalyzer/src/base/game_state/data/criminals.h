#pragma once
#include <array>

#include "base/documents_v2/doc_class.h"
#include "base/document_data/photo.h"

namespace paplease {
	namespace analysis {
		namespace data {

			class CriminalData
			{
			public:
				CriminalData() = default;

				friend CriminalData CreateCriminalData(const documents::v2::Doc& document);
			private:
				static constexpr size_t CriminalCapacity = 3;
			private:
				std::array<documents::data::Photo, CriminalData::CriminalCapacity> m_photos;
			};

			CriminalData CreateCriminalData(const documents::v2::Doc& document);

		}  // namespace data
	}  // namespace analysis
}  // namespace paplease
