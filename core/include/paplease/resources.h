#ifndef PAPLEASE_RESOURCES_H
#define PAPLEASE_RESOURCES_H

#include "opencv2/core/mat.hpp"
#include <filesystem>

namespace resources
{

void init(const std::filesystem::path& asset_root);

const cv::Mat& get_booth_doc_area_mask();

}

#endif // PAPLEASE_RESOURCES_H
