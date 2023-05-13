#pragma once

typedef struct {
    size_t motors_count;
    MotorAttributes *attributes;
} MotorsWrapper;

int config_manager_init_from_file(char *path, MotorsWrapper *wrapper)

int config_manager_parse_config(FILE **f, ConfigWrapper *config_wrapper);

int config_manager_init_motors(Config *config, MotorsWrapper *wrapper);