#include <algorithm>
#include <cctype>
#include <filesystem>

#include <opencv2/imgcodecs.hpp>

static bool is_image_file(const std::string &path)
{
	if (!std::filesystem::exists(path) ||
	    !std::filesystem::is_regular_file(path))
		return false;

	std::string ext = std::filesystem::path(path).extension();
	std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
	if (ext == ".jpg" || ext == ".jpeg" || ext == ".png" || ext == ".tif" ||
	    ext == ".tiff" || ext == ".bmp" || ext == ".webp")
		return true;
	return false;
}

bool read_image(cv::Mat &out, const std::string &path)
{
	if (!is_image_file(path))
		return false;

	cv::imread(path, out);
	return !out.empty();
}
