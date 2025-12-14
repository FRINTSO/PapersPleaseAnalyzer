#include "pch.h"
#include <filesystem>

#include <paplease/game_screen.h>

int main(int argc, char *argv[])
{
	// Resolve asset path relative to executable or from config
	std::filesystem::path exe_dir =
		std::filesystem::path(argv[0]).parent_path();
	std::filesystem::path asset_root = exe_dir / "assets";

	// Or from environment/config:
	// const char* env = std::getenv("PAPLEASE_ASSETS");
	// if (env) asset_root = env;

	game_screen gs;
	if (load_game_screen_from_file(gs, "c:/my_image.png")) {
	}

	return 0;
}
