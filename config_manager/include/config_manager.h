#pragma once

#include "config.h"
#include "stdio.h"

typedef enum {
    INIT_SUCCESS,
    INIT_FAIL
} InitResult;

InitResult config_manager_init_from_file(char *path, MotorsWrapper *wrapper);