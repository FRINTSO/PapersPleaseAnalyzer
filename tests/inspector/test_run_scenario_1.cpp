#include "opencv2/imgcodecs.hpp"
#include "paplease/game_screen.h"
#include "paplease/inspector.h"
#include "test.h"
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <format>
#include <filesystem>
#include <paplease/resources.h>
#include <string>

namespace fs = std::filesystem;

fs::path sim_scenario(std::string_view scenario)
{
	return resources::asset_path() / "game_sim" / scenario;
}

int count_frames(const fs::path &sim_path)
{
	int count = 0;
	for (const auto &entry : fs::directory_iterator(sim_path)) {
		if (entry.is_regular_file()) {
			count++;
		}
	}
	return count;
}

bool get_next_frame(game_screen &screen)
{
	static fs::path scenario_path = sim_scenario("3");
	static int frame_max = count_frames(scenario_path);
	static int frame_count = 0;

	if (frame_count < frame_max) {
		frame_count++;
		fprintf(stderr, "..scanning %i\n", frame_count);
		std::string file = std::format("game_{}.png", frame_count);
		auto frame_path = scenario_path / file;
		return load_game_screen_from_file(screen, frame_path);
	}
	return false;
}

int main()
{
	resources::init("/home/wlm/dev/PapersPleaseAnalyzer/images");

	inspector ins;
	game_screen screen;
	while (get_next_frame(screen)) {
		ins.process_game_frame(screen);
		std::cin.get();
	}
}
