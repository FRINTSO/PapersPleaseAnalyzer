#include "cmd.h"

#include <iostream>
#include <filesystem>

#include <sys/ioctl.h>
#include <unistd.h>

#include <paplease/inspector.h>
#include <paplease/game_screen.h>


static inline int scenario_frames(const std::string &path)
{
	int count = 0;
	for (const auto &entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file()) {
			count++;
		}
	}
	return count;
}

static bool next_scenario_frame(game_screen &screen,
				std::filesystem::path &image_path,
				const std::string &scenario_path)
{
	static auto fileCount = scenario_frames(scenario_path) + 1;
	static int count = 1;

	if (count < fileCount) {
		std::cout << "..scanning " << count << "\n";
		auto filePath = std::filesystem::path(scenario_path) /
			("game_" + std::to_string(count) + ".png");

		image_path = filePath;
		load_game_screen_from_file(screen, filePath);

		count++;
		return true;
	}

	return false;
}

static void show_image_kitty(const std::filesystem::path &path)
{
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	int img_cols = w.ws_col / 2;
	int img_rows = w.ws_row - 2;
	int img_x = w.ws_col - img_cols; // push to the right half

	printf("\033[2J\033[H");
	fflush(stdout);

	char cmd[512];
	snprintf(cmd, sizeof(cmd), "kitten icat --clear --place %dx%d@%dx0 %s",
		 img_cols, img_rows, img_x, path.c_str());
	system(cmd);
}

int cmd_scenario(const std::string &scenario_path)
{
	inspector ins{};
	ins.inform_player =
		+[](std::string_view msg) { std::cout << msg << '\n'; };

	game_screen gs;
	std::filesystem::path image_path;
	while (next_scenario_frame(gs, image_path, scenario_path)) {
		show_image_kitty(image_path);
		inspector_step(ins, gs);
		std::cin.get();
	}

	return 0;
}
