#pragma once
#include <string>

#include <opencv2/opencv.hpp>

namespace paplease {

	struct GameView
	{
		GameView() = default;
		GameView(const std::string& filename);
		GameView(const cv::Mat& gameView);

		cv::Mat booth;
		cv::Mat inspection;
	};

	GameView GetGameView(const std::string& number);
	bool GetNextGameSimView(const std::string& num, GameView& view);

}  // namespace paplease
