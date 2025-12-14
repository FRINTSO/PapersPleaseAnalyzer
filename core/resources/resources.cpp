#include "paplease/compiler.h"
#include "paplease/ocr.h"
#include <filesystem>
#include <paplease/resources.h>
#include <string_view>

namespace resources
{

struct resourcer {
	std::filesystem::path asset_path;
	bool initialized;
};

static resourcer g_resourcer{};

void init(std::string_view asset_path)
{
	g_resourcer.asset_path = asset_path;
	g_resourcer.initialized = true;
}

const std::filesystem::path &asset_path()
{
	return g_resourcer.asset_path;
}

std::filesystem::path typeface_path(typeface tf)
{
	std::filesystem::path typeface_path =
		g_resourcer.asset_path / "typefaces";
	switch (tf) {
	case typeface::booth:
		return typeface_path / "BoothNumber";
	case typeface::bm_mini:
		return typeface_path / "BM Mini";
	case typeface::mini_kylie:
		return typeface_path / "MiniKylie";
	case typeface::t04b03:
		return typeface_path / "04b03";
	}
	unreachable();
}

}
