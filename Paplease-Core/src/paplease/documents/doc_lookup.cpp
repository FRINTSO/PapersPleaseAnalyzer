#include "pch.h"
#include "paplease/documents/doc_lookup.h"

namespace paplease {
    namespace documents {
        
        std::string DocLookup::ToString() const noexcept
        {
            return std::format("{}:{}",
                               magic_enum::enum_name(documentType),
                               magic_enum::enum_name(passportType));
        }

    }  // namespace documents
}  // namespace paplease
