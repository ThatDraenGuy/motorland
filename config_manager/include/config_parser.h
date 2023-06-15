#pragma once

#include <cyaml/cyaml.h>
#include "config.h"

cyaml_err_t config_parser_parse_config(char *filepath, ConfigWrapper **wrapper);
