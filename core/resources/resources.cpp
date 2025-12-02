#include <filesystem>

#include <paplease/resources.h>
#include <paplease/image.h>

namespace resources
{

static std::filesystem::path g_asset_root;
static bool g_initialized = false;

void init(const std::filesystem::path &asset_root)
{
	if (!std::filesystem::exists(asset_root))
		throw std::runtime_error("Asset root does not exist: " +
					 asset_root.string());

	g_asset_root = asset_root;
	g_initialized = true;
}

static std::filesystem::path asset_path(const char *relative_path)
{
	if (!g_initialized)
		throw std::logic_error(
			"resources::init() must be called before accessing assets");

	return g_asset_root / relative_path;
}

const cv::Mat &get_booth_doc_area_mask()
{
	static cv::Mat mask = []() {
		cv::Mat img;
		auto path = asset_path("images/booth/booth_doc_area.png");
		if (!read_image(img, path.string()))
			throw std::runtime_error(
				"Failed to load booth doc area mask: " +
				path.string());
		return img;
	}();
	return mask;
}

}
