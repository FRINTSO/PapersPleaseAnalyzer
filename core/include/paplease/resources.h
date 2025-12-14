#ifndef PAPLEASE_RESOURCES_H
#define PAPLEASE_RESOURCES_H

#include <filesystem>
#include <string_view>

#include <paplease/ocr.h>

void init_resourcer(std::string_view asset_path);
std::filesystem::path resources_get_typeface_path(typeface tf);

#endif // PAPLEASE_RESOURCES_H
