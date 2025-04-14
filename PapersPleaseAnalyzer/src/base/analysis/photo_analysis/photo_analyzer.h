#pragma once
#include <opencv2/opencv.hpp>

#include "base/documents/doc_type.h"
#include "base/documents/data/photo.h"

namespace paplease {
    namespace analysis {
        
        bool IsSamePerson(const documents::data::Photo& headshot1, const documents::data::Photo& headshot2);

    }  // namespace analysis
}  // namespace paplease
