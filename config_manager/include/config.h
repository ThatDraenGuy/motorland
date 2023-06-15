#pragma once

#include <stdlib.h>
#include "motor_internal.h"

typedef struct {
	int *pins;
	size_t pins_count;
} GpioAttributes;

typedef struct {
	char *device_name;
	int baud_rate;
} UartAttributes;

typedef struct {
	char *motor_name;
	//int motor_type;
	int steps_per_revolution;
	int steps_rev_per_minute;

	GpioAttributes gpio_attributes;
	UartAttributes uart_attributes;

} Config;

typedef struct {
	Config *configs;
	size_t configs_count;
} ConfigWrapper;

typedef struct {
	MotorAttributes *attributes;
	size_t motors_count;
} MotorsWrapper;