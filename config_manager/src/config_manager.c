#pragma "once"

#include "config_manager.h"
#include "config_parser.h"

static MotorAttributes *config_to_attributes(Config config)
{
	if (config.gpio_attributes.pins &&
	    config.gpio_attributes.pins_count == 4) {
		GpioConnectionData data = { config.gpio_attributes.pins[0],
					    config.gpio_attributes.pins[1],
					    config.gpio_attributes.pins[2],
					    config.gpio_attributes.pins[3] };
		int steps_per_revolution = config.steps_per_revolution;
		int32_t speed = 10;
		ConnectionAttributes attributes = { .gpio = data };
		return motor_setup(attributes, steps_per_revolution, speed);
	}
	return NULL;
}

static void config_manager_init_motors(ConfigWrapper *config_wrapper,
				       MotorsWrapper *motors_wrapper)
{
	motors_wrapper = malloc(sizeof(MotorsWrapper));
	motors_wrapper->motors_count = config_wrapper->configs_count;
	motors_wrapper->attributes =
		malloc(sizeof(MotorAttributes) * motors_wrapper->motors_count);
	for (size_t i = 0; i < motors_wrapper->motors_count; ++i) {
		motors_wrapper->attributes[i] =
			*config_to_attributes(config_wrapper->configs[i]);
	}
}

InitResult config_manager_init_from_file(char *path,
					 MotorsWrapper *motors_wrapper)
{
	ConfigWrapper *config_wrapper;
	cyaml_err_t err = config_parser_parse_config(path, &config_wrapper);
	if (err != CYAML_OK) {
		return INIT_FAIL;
	}
	config_manager_init_motors(config_wrapper, motors_wrapper);
	return INIT_SUCCESS;
}