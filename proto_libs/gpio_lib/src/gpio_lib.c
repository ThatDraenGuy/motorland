#include <stdio.h>
#include <unistd.h>
#include "motor_internal.h"
#include "gpiod.h"

// Define the GPIO pins used to control the stepper motor
// GPIO7 line pins 58 59 60 61
#define CHIP0_DEV "/dev/gpiochip7"
#define PINS_COUNT 4

#define CONSUMER_NAME "gpio-motor"

const int SEQUENCE_MASK = 7;
const int FULL_SEQUENCE[8][4] = {
    { 1, 0, 0, 0 }, { 1, 0, 1, 0 }, { 0, 0, 1, 0 }, { 0, 1, 1, 0 },
    { 0, 1, 0, 0 }, { 0, 1, 0, 1 }, { 0, 0, 0, 1 }, { 1, 0, 0, 1 },
};

struct ProtoSpecificInfo {
	struct gpiod_chip *chip;
	struct gpiod_line_bulk *lines;
	unsigned int pin_offsets[PINS_COUNT];
	int disable_values[PINS_COUNT];
};

void motor_rotate(struct MotorAttributes *motor, int steps_to_move)
{
	motor->rotate(motor, steps_to_move);
}

void motor_deactivate(struct MotorAttributes *motor)
{
	motor->deactivate(motor);
}

static void rotate(struct MotorAttributes *motor, int steps_to_move);

static void step_motor(struct gpiod_line_bulk *lines, const int pin_values[4]);

static void deactivate(struct MotorAttributes *motor);

struct MotorAttributes *motor_setup(ConnectionAttributes connection, int steps_per_revolution, uint32_t speed)
{
	int ret;
	struct MotorAttributes *motor = malloc(sizeof(struct MotorAttributes));
	if (!motor) {
		perror("Failed to allocate memory for motor");
		return NULL;
	}

	motor->info->lines = malloc(sizeof(struct gpiod_line_bulk));

	motor->info->chip = gpiod_chip_open(CHIP0_DEV);
	;
	if (!motor->info->chip) {
		perror("Failed to open GPIO chip");
		return NULL;
	}

        motor->info->pin_offsets[0] = connection.gpio.pins[0];
        motor->info->pin_offsets[1] = connection.gpio.pins[1];
        motor->info->pin_offsets[2] = connection.gpio.pins[2];
        motor->info->pin_offsets[3] = connection.gpio.pins[3];
        motor->deactivate = &deactivate;
	motor->rotate = &rotate;
	motor->steps_per_revolution = steps_per_revolution;
	motor->step_delay = 60L * 1000L * 1000L / steps_per_revolution / speed;
	const int disabled_values[] = { 0, 0, 0, 0 };
	ret = gpiod_chip_get_lines(motor->info->chip, motor->info->pin_offsets, 4,
				   motor->info->lines);
	if (ret < 0) {
		perror("Failed to get lines");
		return NULL;
	}

	ret = gpiod_line_request_bulk_output(motor->info->lines, CONSUMER_NAME,
					     disabled_values);
	if (ret < 0) {
		perror("Failed to set output bulk");
		return NULL;
	}

	return motor;
}

static void rotate(struct MotorAttributes *motor, int steps_to_move)
{
	int steps_left = abs(steps_to_move); // how many steps to take
	int direction;

	if (steps_to_move > 0)
		direction = 1;
	else
		direction = -1;

	while (steps_left-- > 0) {
		motor->current_steps += direction;
		if (motor->current_steps == motor->steps_per_revolution) {
			motor->current_steps = 0;
		} else if (motor->current_steps == 0) {
			motor->current_steps = motor->steps_per_revolution;
		}
		step_motor(motor->info->lines,
			   FULL_SEQUENCE[motor->current_steps & SEQUENCE_MASK]);
		usleep(motor->step_delay);
	}
}

static void step_motor(struct gpiod_line_bulk *lines, const int pin_values[4])
{
	int ret = gpiod_line_set_value_bulk(lines, pin_values);
	if (ret < 0) {
		perror("Failed to set bulk values");
		return;
	}
}

static void deactivate(struct MotorAttributes *motor)
{
	int ret = gpiod_line_set_value_bulk(motor->info->lines,
					    motor->info->disable_values);
	if (ret < 0) {
		perror("Failed to set pins to zeroes\n");
	}

	gpiod_line_release_bulk(motor->info->lines);
	gpiod_chip_close(motor->info->chip);
	free(motor->info->lines);
	free(motor);
}
