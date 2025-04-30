#include "pch.h"
#include "paplease/game_view.h"

#include <filesystem>

namespace paplease {

	GameView::GameView(const std::string& filename)
		: GameView(std::move(cv::imread(filename)))
	{}

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

	GameView GetGameView(const std::string& number)
	{
		auto path = "c:/dev/PapersPleaseAnalyzer/PapersPleaseAnalyzer/images/game_" + number + ".png";
		return GameView(path);
	}

	bool GetNextGameSimView(const std::string& num, GameView& view)
	{
		auto path = "c:/dev/PapersPleaseAnalyzer/PapersPleaseAnalyzer/images/game_sim/" + num;

		static auto fileCount = GetGameSimCount(path) + 1;
		static int count = 1;


		if (count < fileCount)
		{
			//LOG_RAW("[Game view: {}]", count);
			LOG_RAW("..scanning {}", count);
			//LOG_RAW("scanning game view {}...", count);
			auto filePath = path + "/game_" + std::to_string(count) + ".png";

			view = GameView(filePath);
			count++;
			return true;
		}

		return false;
	}

}  // namespace paplease
