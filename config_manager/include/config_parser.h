#pragma once
#include "config.h"

// TODO: what format and library will be used?
int config_parser_parse_config(char* filepath, ConfigWrapper **wrapper);

void config_parser_print_config(int err, ConfigWrapper *wrapper);

