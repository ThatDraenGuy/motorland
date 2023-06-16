#pragma once
#include "config.h"

int config_parser_parse_config(char *filepath, ConfigWrapper **wrapper);

void config_parser_print_config(int err, ConfigWrapper *wrapper);
