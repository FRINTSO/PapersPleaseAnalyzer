#include "pch.h"
#include "base/game_view.h"



GameView::GameView(const std::string& filename) {
		auto gameView = cv::imread(filename);

#if IS_DOWNSCALED
		cv::Mat boothMat = DownScale(gameView(cv::Rect(1, 237, 356, 434)), SCALE);
		inspection = DownScale(gameView(cv::Rect(357, 237, 784, 434)), SCALE);
#else
		cv::Mat boothMat = gameView(cv::Rect(1, 237, 356, 434));
		inspection = gameView(cv::Rect(357, 237, 784, 434));
#endif
		booth = Booth{ boothMat };
}

GameView GetGameView(const std::string& number) {
	auto path = "c:/dev/PapersPleaseAnalyzer/PapersPleaseAnalyzer/images/game_" + number + ".png";
	return GameView(path);
}