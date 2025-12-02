#pragma once
#include "paplease/core/fixed.h"
#include "paplease/documents/doc_data.h"

namespace paplease {
    namespace analysis {
        namespace data {

            class Profile
            {
            public:
                static constexpr int MatchingData = 1;
                static constexpr int NoData = 0;
                static constexpr int MismatchingData = -1;

                bool RegisterData(const documents::Field& fieldData)
                {
                    auto result = this->CompareData(fieldData);
                    if (result == MismatchingData)
                    {
                        return false;
                    }
                    else if (result == NoData)
                    {
                        m_fields.Set(fieldData.Category(), fieldData);
                    }

                    return true;
                }
            private:
                int CompareData(const documents::Field& fieldData) const
                {
                    if (!m_fields.Contains(fieldData.Category()))
                    {
                        return NoData;
                    }

                    const auto& storedData = m_fields[fieldData.Category()];

                    bool isIdenticalData = storedData.GetData() == fieldData.GetData();

                    return isIdenticalData ? MatchingData : MismatchingData;
                }

                static constexpr size_t FieldsCapacity = 20;
            private:
                core::FixedHashTable<documents::FieldCategory, documents::Field, FieldsCapacity> m_fields;
            };

        }
    }
}
