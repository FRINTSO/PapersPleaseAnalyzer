#pragma once
#include <opencv2/opencv.hpp>

#include "paplease/documents/doc_type.h"
#include "paplease/documents/data/photo.h"

namespace paplease {
    namespace analysis_v1 {
        
        bool IsSamePerson(const documents::data::Photo& headshot1, const documents::data::Photo& headshot2);

    }  // namespace analysis_v1
}  // namespace paplease
