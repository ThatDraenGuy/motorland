#pragma once

#include "config.h"
#include "stdio.h"

int config_manager_init_from_file(char *path, MotorsWrapper *wrapper);

int config_manager_parse_config(char *path, ConfigWrapper **config_wrapper);

int config_manager_init_motors(Config *config, MotorsWrapper *wrapper);