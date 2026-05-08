#include "cmd.h"

#include "opencv2/imgcodecs.hpp"
#include "paplease/game_screen.h"
#include <iostream>

#include <sys/ioctl.h>
#include <unistd.h>

#include <paplease/capture.h>
#include "paplease/inspector.h"
#include "paplease/resources.h"

constexpr int TWO_SECONDS = 200000;
static const std::string TMP_FRAME = "/tmp/paplease_frame.png";

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

int cmd_live(const resources_ctx &ctx)
{
	inspector ins{};
	ins.inform_player =
		+[](std::string_view msg) { std::cout << msg << '\n'; };

	game_screen gs;
	while (true) {
		cv::Mat buffer = capture_window("PapersPlease");
		if (load_game_screen_from_buffer(gs, buffer)) {
			cv::imwrite(TMP_FRAME, gs.pixels);
			show_image_kitty(TMP_FRAME);
			inspector_step(ins, gs, ctx);
		} else {
			std::cout << "Cannot see screen\n";
		}
		usleep(TWO_SECONDS);
	}

	return 0;
}
