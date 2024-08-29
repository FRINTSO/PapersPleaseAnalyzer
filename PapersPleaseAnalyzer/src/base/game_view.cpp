#include "pch.h"
#include "base/game_view.h"

#include "base/common.h"
#include "base/image_process.h"

GameView::GameView(const std::string& filename) {
#if	USE_GRAY
	auto gameView = ToGrayscale(cv::imread(filename));
#else
		auto gameView = cv::imread(filename);
#endif
#if IS_DOWNSCALED
		booth = ScaleImage(gameView(cv::Rect(1, 237, 356, 434)), SCALE);
		inspection = ScaleImage(gameView(cv::Rect(357, 237, 784, 434)), SCALE);
#else
		booth = gameView(cv::Rect(1, 237, 356, 434));
		inspection = gameView(cv::Rect(357, 237, 784, 434));
#endif
}

GameView GetGameView(const std::string& number) {
	auto path = "c:/dev/PapersPleaseAnalyzer/PapersPleaseAnalyzer/images/game_" + number + ".png";
	return GameView(path);
}

