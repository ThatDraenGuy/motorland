#include "motor_internal.h"
#include <string.h>

const uint32_t MAX_SPEED = 600;
const int32_t MAX_STEPS = 1024;
const int32_t MIN_STEPS = -1024;

struct Params {
	int32_t steps;
	uint32_t speed;
	uint32_t motor_index;
};

bool are_params_valid(struct Params *params)
{
	if (params->speed > MAX_SPEED || params->speed < 0) {
		fprintf(stderr, "Invalid speed: %d. It should be less than %u\n", params->speed, MAX_SPEED);
		return false;
	}

	if (params->steps > MAX_STEPS || params->steps < MIN_STEPS) {
		fprintf(stderr, "Invalid number of steps: %d. It should be in range (%d, %d)\n", params->steps, MIN_STEPS, MAX_STEPS);
		return false;
	}

	if (params->motor_index != 0 && params->motor_index != 1) {
		fprintf(stderr, "Invalid motor index: %u. It should be 0 or 1\n",
			params->motor_index);
		return false;
	}
	return true;
}

struct Params *read_params(int argc, char *argv[])
{
	struct Params *params = malloc(sizeof(struct Params));
	if (params == NULL) {
		perror("Params memory allocation failed");
		exit(EXIT_FAILURE);
	}

	params->steps = 10;
	params->speed = 5;
	params->motor_index = 0;
	int c;

	while ((c = getopt(argc, argv, "n:s:m:")) != -1) {
		switch (c) {
		case 'n':
			params->steps = (int32_t) strtol(optarg, NULL, 10);
			break;
		case 's':
			params->speed = strtoul(optarg, NULL, 10);
			break;
		case 'm':
			params->motor_index = strtoul(optarg, NULL, 10);
			break;
		default:
			printf("Invalid Argument %c\n", c);
			printf("Usage : %s\n"
			       "\t -n number of steps (positive or negative)\n"
			       "\t -s Speed step (default 5)\n"
			       "\t -m Motor index (default 0)\n",
			       argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	return params;
}

int main(int argc, char *argv[])
{
	struct Params *params = read_params(argc, argv);

	if (!are_params_valid(params)) {
		return 1;
	}

	ConnectionAttributes connection;
	GpioConnectionData vertical_motor_conf = { .pins = { 58, 60, 59, 61 } };
	GpioConnectionData horizontal_motor_conf = { .pins = { 31, 33, 32,
							       34 } };
	GpioConnectionData conf[2];
	conf[0] = vertical_motor_conf;
	conf[1] = horizontal_motor_conf;

	int steps_per_revolution = 2038;
	memcpy(connection.gpio.pins, &conf[params->motor_index].pins,
	       sizeof(conf[params->motor_index].pins));
	struct MotorAttributes *motor =
		motor_setup(connection, steps_per_revolution, params->speed);
	if (!motor) {
		fprintf(stderr, "Failed to initialize motor: %d\n",
			params->motor_index);
		return 1;
	}
	motor_rotate(motor, params->steps);

	free(params);
	motor_deactivate(motor);

	return 0;
}
