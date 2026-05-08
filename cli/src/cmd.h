#ifndef PAPLEASE_CLI_CMD_H
#define PAPLEASE_CLI_CMD_H

#include <string>

#include <paplease/resources.h>

int cmd_scan(const std::string &image_path, const resources_ctx &ctx);
int cmd_scenario(const std::string &scenario_name, const resources_ctx &ctx);
int cmd_live(const resources_ctx& ctx);

#endif // PAPLEASE_CLI_CMD_H
