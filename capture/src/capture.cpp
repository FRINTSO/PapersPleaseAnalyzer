#include <paplease/capture.h>

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <opencv2/imgproc.hpp>

static std::string find_window_stable_id(const std::string &window_class)
{
	FILE *pipe = popen("hyprctl clients -j", "r");
	if (!pipe)
		return {};

	std::string json;
	char buf[4096];
	while (size_t n = fread(buf, 1, sizeof(buf), pipe))
		json.append(buf, n);
	pclose(pipe);

	auto pos = json.find("\"class\": \"");
	while (pos != std::string::npos) {
		auto val_start = pos + 10;
		auto val_end = json.find('"', val_start);
		if (val_end == std::string::npos)
			break;

		std::string cls =
			json.substr(val_start, val_end - val_start);
		if (cls.find(window_class) != std::string::npos) {
			auto id_pos = json.find("\"stableId\": \"", pos);
			if (id_pos == std::string::npos)
				break;

			auto id_val_start = id_pos + 13;
			auto id_val_end =
				json.find('"', id_val_start);
			if (id_val_end == std::string::npos)
				break;

			return json.substr(id_val_start,
					   id_val_end - id_val_start);
		}

		pos = json.find("\"class\": \"", val_end);
	}

	return {};
}

static cv::Mat capture_toplevel_ppm(const std::string &stable_id)
{
	std::string cmd = "grim -T " + stable_id + " -t ppm -";
	FILE *pipe = popen(cmd.c_str(), "r");
	if (!pipe)
		return {};

	char line[256];

	fgets(line, sizeof(line), pipe);
	if (strncmp(line, "P6", 2) != 0) {
		pclose(pipe);
		return {};
	}

	do {
		fgets(line, sizeof(line), pipe);
	} while (line[0] == '#');

	int width, height;
	sscanf(line, "%d %d", &width, &height);

	fgets(line, sizeof(line), pipe);

	std::vector<unsigned char> pixels(width * height * 3);
	size_t total = 0;
	while (total < pixels.size()) {
		size_t n = fread(pixels.data() + total, 1,
				 pixels.size() - total, pipe);
		if (n == 0)
			break;
		total += n;
	}
	pclose(pipe);

	if (total != pixels.size())
		return {};

	cv::Mat rgb(height, width, CV_8UC3, pixels.data());
	cv::Mat bgr;
	cv::cvtColor(rgb, bgr, cv::COLOR_RGB2BGR);
	return bgr;
}

cv::Mat capture_window(const std::string &title)
{
	std::string stable_id = find_window_stable_id(title);
	if (stable_id.empty())
		return {};

	return capture_toplevel_ppm(stable_id);
}
