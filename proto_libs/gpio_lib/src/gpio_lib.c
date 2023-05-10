#include "motor_internal.h"

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

struct MotorAttributes *motor_setup(uint32_t speed)
{
	int ret;
	struct MotorAttributes *motor = malloc(sizeof(struct MotorAttributes));
	if (!motor) {
		perror("Failed to allocate memory for motor");
		return NULL;
	}

	motor->gpio.lines = malloc(sizeof(struct gpiod_line_bulk));

	motor->gpio.chip = gpiod_chip_open(CHIP0_DEV);
	;
	if (!motor->gpio.chip) {
		perror("Failed to open GPIO chip");
		return NULL;
	}

	motor->gpio.pin_offsets[0] = PIN_1;
	motor->gpio.pin_offsets[1] = PIN_3;
	motor->gpio.pin_offsets[2] = PIN_2;
	motor->gpio.pin_offsets[3] = PIN_4;
	motor->deactivate = &deactivate;
	motor->rotate = &rotate;
	motor->steps_per_revolution = STEPS_PER_REVOLUTION;
	motor->step_delay = 60L * 1000L * 1000L / STEPS_PER_REVOLUTION / speed;
	const int disabled_values[] = { 0, 0, 0, 0 };
	ret = gpiod_chip_get_lines(motor->gpio.chip, motor->gpio.pin_offsets, 4,
				   motor->gpio.lines);
	if (ret < 0) {
		perror("Failed to get lines");
		return NULL;
	}

	ret = gpiod_line_request_bulk_output(motor->gpio.lines, CONSUMER_NAME,
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
		step_motor(motor->gpio.lines,
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
	int ret = gpiod_line_set_value_bulk(motor->gpio.lines,
					    motor->gpio.disable_values);
	if (ret < 0) {
		perror("Failed to set pins to zeroes\n");
	}

	gpiod_line_release_bulk(motor->gpio.lines);
	gpiod_chip_close(motor->gpio.chip);
	free(motor->gpio.lines);
	free(motor);
}
