#ifndef PAPLEASE_CLI_CMD_H
#define PAPLEASE_CLI_CMD_H

#include <string>

int cmd_scan(const std::string &image_path);
int cmd_scenario(const std::string &scenario_name);
int cmd_live();

#endif // PAPLEASE_CLI_CMD_H
