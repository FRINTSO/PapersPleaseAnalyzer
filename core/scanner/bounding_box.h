#ifndef _SCANNER_BOUNDING_BOX_H
#define _SCANNER_BOUNDING_BOX_H

#include "opencv2/core/types.hpp"
#include <opencv2/core/mat.hpp>
#include <paplease/documents.h>
#include <paplease/geometry.h>

cv::Rect find_bounding_box(cv::Mat section, doc_variant variant);

#endif // _SCANNER_BOUNDING_BOX_H
