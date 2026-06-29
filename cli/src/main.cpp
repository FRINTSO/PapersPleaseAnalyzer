#include "pch.h"
#include <cstdio>
#include <filesystem>
#include <string_view>

#include <magic_enum/magic_enum.hpp>

#include <paplease/date.h>
#include <paplease/documents.h>
#include <paplease/game_screen.h>

#include "cmd.h"

static void usage(const char *argv0)
{
	fprintf(stderr, "Usage: %s <command> <image>\n", argv0);
	fprintf(stderr, "Commands:\n");
	fprintf(stderr, "  scan      Scan and parse all documents\n");
	fprintf(stderr, "  scenario  Run inspector on game scenario\n");
	fprintf(stderr, "  live      Run inspector on live game\n");
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		usage(argv[0]);
		return 1;
	}

	std::string_view cmd = argv[1];
	if (argc == 2) {
		if (cmd == "live")
			return cmd_live();
	} else {
		std::string image_path = argv[2];

		if (cmd == "scan")
			return cmd_scan(image_path);
		if (cmd == "scenario")
			return cmd_scenario(argv[2]);
	}

	fprintf(stderr, "Unknown command: %s\n", argv[1]);
	usage(argv[0]);
	return 1;
}
