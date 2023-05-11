#include "config_manager.h"
#include "config_parser.h"
#include "motor_initializer.h"

int config_manager_init_from_file(char *path, MotorsWrapper *wrapper) {
    // TODO: implementation
    FILE f = fopen(path, "r");
    if (!f) {
        return FILE_OPEN_FAILED;
    }
    ConfigWrapper wrapper = {0};
    int parse_result = config_manager_parse_config(f, wrapper);
    if (parse_result != SUCCESS) {
        return parse_result;
    }
    return config_manager_init_motors(config, wrapper);
}

int config_manager_parse_config(FILE **f, ConfigWrapper *config_wrapper) {
    // TODO: implementation
    if (!f) {
        *config_wrapper = temp_gpio_wrapper();
        return SUCCESS;
    }
    // TODO: parse config from file

}

int config_manager_init_motors(ConfigWrapper *config_wrapper, MotorsWrapper *wrapper) {
    wrapper->motors_count = config_wrapper->motors_count;
    // TODO: transform structs
}