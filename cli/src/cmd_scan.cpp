#include "cmd.h"

#include <cstdio>

#include <paplease/game_screen.h>
#include <paplease/inspector.h>
#include <paplease/observation.h>
#include <paplease/resources.h>

int cmd_scan(const std::string &image_path, const resources_ctx &ctx)
{
	game_screen gs;
	if (!load_game_screen_from_file(gs, image_path)) {
		fprintf(stderr, "Failed to load: %s\n", image_path.c_str());
		return 1;
	}

	observe_options opts{ .skip_rulebook = true };
	observation obs = observe_frame(gs, ctx, opts);

	if (obs.entrant_docs.visible.empty()) {
		printf("No documents found.\n");
	}

	return 0;
}



