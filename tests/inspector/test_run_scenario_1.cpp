/*
 * test_run_scenario_1.cpp - Run the inspector over a sequence of game frames
 *
 * Integration test that feeds screenshots through inspector_step().
 * Requires scenario data in TEST_DATA_DIR/game_sim/3/
 */

#include <cstdio>
#include <format>
#include <filesystem>
#include <iostream>
#include <string>
#include <string_view>

#include <paplease/game_screen.h>
#include <paplease/inspector.h>

#include "test.h"

namespace fs = std::filesystem;

static fs::path scenario_dir()
{
	return fs::path(TEST_DATA_DIR) / "game_sim" / "3";
}

static int count_frames(const fs::path &dir)
{
	int count = 0;
	for (const auto &entry : fs::directory_iterator(dir))
		if (entry.is_regular_file())
			count++;
	return count;
}

int main()
{
	fs::path sim_path = scenario_dir();
	if (!fs::exists(sim_path)) {
		fprintf(stderr, "SKIP: scenario dir not found: %s\n",
			sim_path.c_str());
		return 0;
	}

	int frame_max = count_frames(sim_path);
	if (frame_max == 0) {
		fprintf(stderr, "SKIP: no frames in %s\n", sim_path.c_str());
		return 0;
	}

	inspector ins{};
	ins.inform_player =
		+[](std::string_view msg) { std::cout << msg << '\n'; };

	for (int i = 1; i <= frame_max; i++) {
		std::string file = std::format("game_{}.png", i);
		auto frame_path = sim_path / file;

		game_screen screen;
		if (!load_game_screen_from_file(screen, frame_path))
			continue;

		fprintf(stderr, "..scanning frame %d/%d\n", i, frame_max);
		inspector_step(ins, screen);
	}

	fprintf(stderr, "Scenario 1 integration test passed (%d frames)\n",
		frame_max);
	return 0;
}
