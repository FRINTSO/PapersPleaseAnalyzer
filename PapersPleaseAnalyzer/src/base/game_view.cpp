#include "pch.h"
#include "base/game_view.h"

#include <filesystem>

#include "base/common.h"
#include "base/image_process.h"
#include "base/utils/log.h"

namespace paplease {

	GameView::GameView(const std::string& filename)
	{
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

	GameView GetGameView(const std::string& number)
	{
		auto path = "c:/dev/PapersPleaseAnalyzer/PapersPleaseAnalyzer/images/game_" + number + ".png";
		return GameView(path);
	}

	static inline int GetGameSimCount(const std::string& path)
	{
		int count = 0;
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			if (entry.is_regular_file())
			{
				count++;
			}
		}
		return count;
	}

	bool GetNextGameSimView(const std::string& num, GameView& view)
	{
		auto path = "c:/dev/PapersPleaseAnalyzer/PapersPleaseAnalyzer/images/game_sim/" + num;

		static auto fileCount = GetGameSimCount(path) + 1;
		static int count = 1;


		if (count < fileCount)
		{
			LOG_RAW("[Game view: {}]", count);
			auto filePath = path + "/game_" + std::to_string(count) + ".png";

			view = GameView(filePath);
			count++;
			return true;
		}

		return false;
	}

}  // namespace paplease
