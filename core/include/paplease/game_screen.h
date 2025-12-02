#ifndef PAPLEASE_GAME_VIEW_H
#define PAPLEASE_GAME_VIEW_H

#include <string>

#include <opencv2/core/mat.hpp>

#include <paplease/geometry.h>
#include <paplease/documents.h>

enum class ui_section {
	booth,
	inspection,
};

struct game_screen {
  cv::Mat pixels;
};

cv::Mat slice_section(const game_screen &screen, ui_section section);

bool load_game_screen_from_file(game_screen &out, const std::string &path);
bool load_game_screen_from_buffer(game_screen &out, const cv::Mat &buffer);

#endif // PAPLEASE_GAME_VIEW_H
