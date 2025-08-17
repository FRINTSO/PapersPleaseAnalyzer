#pragma once
#include "paplease/documents/data/photo.h"

namespace paplease {
    namespace analysis {
        
        bool IsSamePerson(const documents::data::Photo& headshot1, const documents::data::Photo& headshot2);

    }  // namespace analysis
}  // namespace paplease
