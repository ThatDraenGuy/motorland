#include "motor_internal.h"
#include <string.h>

struct Params {
	int32_t steps;
	uint32_t speed;
	uint32_t motor_index;
};

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
			params->steps = atoi(optarg);
			break;
		case 's':
			params->speed = atoi(optarg);
			break;
		case 'm':
			params->motor_index = strtoul(optarg, NULL, 10);
			if (params->motor_index > 1 || params->motor_index < 0) {
				params->motor_index = 0;
			}
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
		fprintf(stderr, "Failed to initialize motor: %d\n", params->motor_index);
		return 1;
	}
	motor_rotate(motor, params->steps);

	free(params);
	motor_deactivate(motor);

	return 0;
}
