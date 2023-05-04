#pragma once

int config_manager_init_from_file(char *path, MotorsWrapper wrapper);

int config_manager_parse_config(FILE **f, Config *config);

int config_manager_init_motors(Config *config, MotorsWrapper wrapper);
