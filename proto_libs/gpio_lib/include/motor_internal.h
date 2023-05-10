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
#define PIN_1 2
#define PIN_2 3
#define PIN_3 4
#define PIN_4 5

#define CONSUMER_NAME "gpio-motor"
#define STEPS_PER_REVOLUTION 2038

// Define the delay between steps in microseconds

const int SEQUENCE_MASK = 7;
const int FULL_SEQUENCE[8][4] = {
	{ 1, 0, 0, 0 }, { 1, 0, 1, 0 }, { 0, 0, 1, 0 }, { 0, 1, 1, 0 },
	{ 0, 1, 0, 0 }, { 0, 1, 0, 1 }, { 0, 0, 0, 1 }, { 1, 0, 0, 1 },
};

struct GpioInfo {
	struct gpiod_chip *chip;
	struct gpiod_line_bulk *lines;
	unsigned int pin_offsets[PINS_COUNT];
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
