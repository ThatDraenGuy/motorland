#pragma "once"

#include "../include/config_manager.h"
#include "../include/config_parser.h"

enum {
    FILE_OPEN_FAILED,
    SUCCESS
};

int config_manager_init_from_file(char *path, MotorsWrapper *wrapper) {
    // TODO: implementation
    /*FILE *f = fopen(path, "r");
    if (!f) {
        return FILE_OPEN_FAILED;
    }
    ConfigWrapper config_wrapper = {0};
    int parse_result = config_manager_parse_config(f, config_wrapper);
    if (parse_result != SUCCESS) {
        return parse_result;
    }*/
    return -1;
    // TODO: init motors
}

/*ConfigWrapper temp_gpio_wrapper() {
    GPIO_attributes attributes = {.pins = {2, 4, 3, 5}};
    Config config = {
            .motor_name="gpio-motor",
            .motor_type=GPIO,
            .steps_per_revolution=2038,
            .steps_per_minute=10,
            .attributes=attributes};
    ConfigWrapper wrapper = {.config=config, .motors_count=1};
    return config;
}*/

int config_manager_parse_config(char *filepath, ConfigWrapper **config_wrapper) {
    // TODO: implementation
    return config_parser_parse_config(filepath, config_wrapper);
}