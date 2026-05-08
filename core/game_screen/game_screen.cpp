#include "opencv2/imgproc.hpp"
#include "paplease/colorspace.h"
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

static rectangle find_game_content(const cv::Mat &buffer);

void extract_game_screen(game_screen &out, const cv::Mat &buffer)
{
	if (buffer.cols == 1142 && buffer.rows == 672) {
		out.pixels = buffer(
			cv::Rect(1, 1, GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT));
		return;
	}

	rectangle content = find_game_content(buffer);

	if (content.empty())
		return;
	cv::Mat cropped = buffer(content.to_cv());
	if (cropped.cols == GAME_SCREEN_WIDTH &&
	    cropped.rows == GAME_SCREEN_HEIGHT) {
		out.pixels = cropped;
		return;
	}
	cv::resize(cropped, out.pixels,
		   cv::Size(GAME_SCREEN_WIDTH, GAME_SCREEN_HEIGHT), 0, 0,
		   cv::INTER_AREA);

	fprintf(stderr, "cols: %i, rows: %i\n", buffer.cols, buffer.rows);
}

static rectangle find_game_content(const cv::Mat &buffer)
{
	constexpr hsv_range non_black{ 0, 179, 0, 255, 10, 255 };
	cv::Mat mask = bgr_to_hsv_mask(buffer, non_black);
	// Scan inward from each edge to find where the solid black border ends
	int top = 0, bottom = mask.rows - 1;
	int left = 0, right = mask.cols - 1;
	while (top < bottom && cv::countNonZero(mask.row(top)) == 0)
		top++;
	while (bottom > top && cv::countNonZero(mask.row(bottom)) == 0)
		bottom--;
	while (left < right && cv::countNonZero(mask.col(left)) == 0)
		left++;
	while (right > left && cv::countNonZero(mask.col(right)) == 0)
		right--;
	return { left, top, right - left + 1, bottom - top + 1 };
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
