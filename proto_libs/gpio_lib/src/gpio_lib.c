
#include "motor_internal.h"

const int SEQUENCE_MASK = 7;
const int FULL_SEQUENCE[8][4] = {
	{ 1, 0, 0, 0 }, { 1, 0, 1, 0 }, { 0, 0, 1, 0 }, { 0, 1, 1, 0 },
	{ 0, 1, 0, 0 }, { 0, 1, 0, 1 }, { 0, 0, 0, 1 }, { 1, 0, 0, 1 },
};

void motor_rotate(struct MotorAttributes *motor, int steps_to_move)
{
	motor->rotate(motor, steps_to_move);
}

void motor_deactivate(struct MotorAttributes *motor)
{
	motor->deactivate(motor);
}

typedef struct {
	uint8_t chip;
	uint8_t offset;
} PinInfo;

static PinInfo gpio_num_to_pin_info(uint8_t gpio_num)
{
	uint8_t chip = gpio_num / 8;
	uint8_t offset = gpio_num % 8;
	PinInfo pin_info = { .chip = chip, .offset = offset };
	return pin_info;
}

static void rotate(struct MotorAttributes *motor, int steps_to_move);

static void step_motor(struct gpiod_line_bulk *lines, const int pin_values[4]);
static void deactivate(struct MotorAttributes *motor);

struct MotorAttributes *motor_setup(ConnectionAttributes connection,
				    int steps_per_revolution, uint32_t speed)
{
	int ret;
	struct MotorAttributes *motor = malloc(sizeof(struct MotorAttributes));
	if (!motor) {
		perror("Failed to allocate memory for motor");
		return NULL;
	}

	motor->deactivate = &deactivate;
	motor->rotate = &rotate;
	motor->steps_per_revolution = steps_per_revolution;
	motor->step_delay = 60L * 1000L * 1000L / steps_per_revolution / speed;

	motor->gpio.lines = malloc(sizeof(struct gpiod_line_bulk));
	if (!motor->gpio.lines) {
		perror("Failed to allocate memory for gpiod_line_bulk");
		goto line_err;
	}

	// Initialize the gpiod_line_bulk
	gpiod_line_bulk_init(motor->gpio.lines);

	// Iterate over the pins and add the corresponding lines
	for (int i = 0; i < PINS_COUNT; i++) {
		int pin = connection.gpio.pins[i];

		// Convert pin to chip and offset pair
		PinInfo pin_info = gpio_num_to_pin_info(pin);

		// Open the GPIO chip
		char chip_path[20];
		snprintf(chip_path, sizeof(chip_path), "/dev/gpiochip%d",
			 pin_info.chip);
		struct gpiod_chip *chip = gpiod_chip_open(chip_path);
		if (!chip) {
			perror("Failed to open GPIO chip");
			fprintf(stderr, "Path: %s\n", chip_path);
			goto err;
		}
		motor->gpio.chips[i] = chip;

		// Get the line for the current pin
		struct gpiod_line *line =
			gpiod_chip_get_line(chip, pin_info.offset);
		if (!line) {
			perror("Failed to get GPIO line");
			gpiod_chip_close(chip);
			goto err;
		}

		// Add the line to the gpiod_line_bulk
		gpiod_line_bulk_add(motor->gpio.lines, line);

		// Set the direction of the line to output
		ret = gpiod_line_request_output(line, CONSUMER_NAME, 0);

		if (ret < 0) {
			perror("Failed to set output bulk");
			gpiod_chip_close(chip);
			goto err;
		}
	}
	return motor;

err:
	free(motor->gpio.lines);
line_err:
	free(motor);
	return NULL;
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
	for (int i = 0; i < PINS_COUNT; ++i) {
		struct gpiod_line *line = gpiod_line_bulk_get_line(lines, i);
		int ret = gpiod_line_set_value(line, pin_values[i]);
		if (ret < 0) {
			perror("Failed to set gpio value to step");
			return;
		}
	}
}
static void deactivate(struct MotorAttributes *motor)
{
	for (int i = 0; i < PINS_COUNT; ++i) {
		struct gpiod_line *line =
			gpiod_line_bulk_get_line(motor->gpio.lines, i);
		int ret = gpiod_line_set_value(line,
					       motor->gpio.disable_values[i]);
		if (ret < 0) {
			perror("Failed to set pins to zeroes\n");
		}
		gpiod_line_release(line);
	}

	for (int i = 0; i < PINS_COUNT; ++i) {
		struct gpiod_chip *chip = motor->gpio.chips[i];
		if (chip) {
			gpiod_chip_close(chip);
		}
	}
	free(motor->gpio.lines);
	free(motor);
}
