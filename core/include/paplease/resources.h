#ifndef PAPLEASE_RESOURCES_H
#define PAPLEASE_RESOURCES_H

#include <filesystem>
#include <string_view>

#include <paplease/ocr.h>

namespace resources {

void init(std::string_view asset_path);
const std::filesystem::path& asset_path();
std::filesystem::path typeface_path(typeface tf);

}

#endif // PAPLEASE_RESOURCES_H
