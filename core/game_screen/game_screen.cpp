#include <algorithm>
#include <cctype>
#include <cstdio>
#include <filesystem>
#include <string>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/core/mat.hpp>

#include <paplease/game_screen.h>
#include <paplease/geometry.h>
#include <paplease/compiler.h>

// Screen geometry constants
static constexpr int GAME_SCREEN_WIDTH = 1140;
static constexpr int GAME_SCREEN_HEIGHT = 670;
static constexpr int BOOTH_X = 0;
static constexpr int BOOTH_Y = 236;
static constexpr int BOOTH_WIDTH = 356;
static constexpr int BOOTH_HEIGHT = 434;
static constexpr rectangle BOOTH_RECT{ BOOTH_X, BOOTH_Y, BOOTH_WIDTH,
				       BOOTH_HEIGHT };
static constexpr int INSPECTION_X = 356;
static constexpr int INSPECTION_Y = 236;
static constexpr int INSPECTION_WIDTH = 784;
static constexpr int INSPECTION_HEIGHT = 434;
static constexpr rectangle INSPECTION_RECT{ INSPECTION_X, INSPECTION_Y,
					    INSPECTION_WIDTH,
					    INSPECTION_HEIGHT };


void extract_game_screen(game_screen &out, const cv::Mat &buffer)
{
	if (buffer.cols == 1142 && buffer.rows == 672) {
		out.pixels = buffer(cv::Rect(1, 1, GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT));
		return;
	}

	fprintf(stderr, "cols: %i, rows: %i\n", buffer.cols, buffer.rows);

	TODO("implement extraction logic");
}

bool validate_game_screen(const game_screen &screen)
{
	return screen.pixels.cols == GAME_SCREEN_WIDTH &&
	       screen.pixels.rows == GAME_SCREEN_HEIGHT;
}

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

static bool read_image(cv::Mat &out, const std::string &path)
{
	if (!is_image_file(path))
		return false;

	cv::imread(path, out);
	return !out.empty();
}
bool load_game_screen_from_file(game_screen &out, const std::string &path)
{
	cv::Mat img;
	if (!read_image(img, path))
		return false;

	extract_game_screen(out, img);
	return validate_game_screen(out);
}

bool load_game_screen_from_buffer(game_screen &out, const cv::Mat &buffer)
{
	if (buffer.empty())
		return false;

	extract_game_screen(out, buffer);
	return validate_game_screen(out);
}

constexpr rectangle rectangle_from_ui_section(ui_section section)
{
	switch (section) {
	case ui_section::booth:
		return BOOTH_RECT;
	case ui_section::inspection:
		return INSPECTION_RECT;
	}
	unreachable();
}

cv::Mat slice_section(const game_screen &screen, ui_section section)
{
	rectangle section_box = rectangle_from_ui_section(section);
	cv::Rect roi(section_box.x, section_box.y, section_box.width,
		     section_box.height);
	return { screen.pixels(roi) };
}
