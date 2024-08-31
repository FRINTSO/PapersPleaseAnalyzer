#pragma once
#include <array>

#include "base/documents_v2/doc_class.h"
#include "base/document_data/photo.h"

class CriminalData
{
public:
	CriminalData() = default;

	friend CriminalData CreateCriminalData(const Documents::V2::Doc& document);
private:
	static constexpr size_t CriminalCapacity = 3;
private:
	std::array<Documents::Data::Photo, CriminalData::CriminalCapacity> m_photos;
};

CriminalData CreateCriminalData(const Documents::V2::Doc& document);