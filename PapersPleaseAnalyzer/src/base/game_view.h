#pragma once
#include <string>

#include <opencv2/opencv.hpp>

#include "base/booth.h"

struct GameView {
	GameView(const std::string& filename);

	Booth booth;
	cv::Mat inspection;
};

GameView GetGameView(const std::string& number);