#pragma once

#include "motor_api.h"
#include <gpiod.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Define the GPIO pins used to control the stepper motor
// GPIO7 line pins 58 59 60 61
#define CHIP0_DEV "/dev/gpiochip7"
#define PINS_COUNT 4

#define CONSUMER_NAME "gpio-motor"

// Define the delay between steps in microseconds

struct GpioInfo {
	struct gpiod_chip *chips[PINS_COUNT];
	struct gpiod_line_bulk *lines;
	int disable_values[PINS_COUNT];
};

typedef void (*rotate_fn)(struct MotorAttributes *motor, int steps_to_move);

typedef void (*deactivate_fn)(struct MotorAttributes *motor);

struct MotorAttributes {
	rotate_fn rotate;
	deactivate_fn deactivate;
	struct GpioInfo gpio;
	uint32_t current_steps;
	uint32_t steps_per_revolution;
	uint16_t step_delay;
};
