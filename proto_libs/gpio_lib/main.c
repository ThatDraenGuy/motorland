#include "motor_internal.h"

struct Params {
	int32_t steps;
	uint32_t speed;
};

struct Params *read_params(int argc, char *argv[])
{
	struct Params *params = malloc(sizeof(struct Params));
	params->steps = 10;
	params->speed = 5;
	int c;

	while ((c = getopt(argc, argv, "n:s")) != -1) {
		switch (c) {
		case 'n':
			params->steps = atoi(optarg);
			break;
		case 's':
			params->speed = atoi(optarg);
			break;
		default:
			printf("Invalid Argument %c\n", c);
			printf("Usage : %s\n"
			       "\t -n number of steps (positive or negative)\n"
			       "\t -s Speed step (default 5)\n",
			       argv[0]);
			exit(EXIT_FAILURE);
		}
	}
	return params;
}

int main(int argc, char *argv[])
{
	struct Params *params = read_params(argc, argv);

	struct MotorAttributes *motor = motor_setup(params->speed);
	motor_rotate(motor, params->steps);

	free(params);
	motor_deactivate(motor);

	return 0;
}
