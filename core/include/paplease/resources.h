#ifndef PAPLEASE_RESOURCES_H
#define PAPLEASE_RESOURCES_H

#include <array>
#include <filesystem>
#include <string_view>
#include <unordered_map>

#include <paplease/types.h>

struct resources_ctx {
	std::filesystem::path asset_root;
	std::array<std::unordered_map<u64, char>, 4> charsets;
};

resources_ctx make_resources(std::string_view root);

#endif // PAPLEASE_RESOURCES_H
