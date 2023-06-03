#include "motor_internal.h"

#include <stdlib.h>
#include <stdio.h>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

#define MOTOR_STATUS 0xC0046D02
#define MOTOR_CMD 0xC0046D01

struct ProtoSpecificInfo {
	char *motor_device;
};

typedef struct {
	int command;
	int x_steps;
	int y_steps;
} UartDriverPayload;

enum DriverCommands { STOP = 0, DOWN = 1, UP = 2, RIGHT = 3, LEFT = 4 };

static void rotate(struct MotorAttributes *motor, int steps_to_move);
static void deactivate(struct MotorAttributes *motor);
static void motor_ioctl(char *dev, int cmd, int *arg);
static void send_command(char *dev, int cmd, int sspeed);

MotorAttributes *motor_setup(ConnectionAttributes connection,
			     int steps_per_revolution, uint32_t speed)
{
	MotorAttributes *motor = malloc(sizeof(struct MotorAttributes));
	if (!motor) {
		perror("Failed to allocate memory for motor");
		return NULL;
	}

	motor->info->motor_device = connection.uart.uart_dev_name;
	motor->deactivate = &deactivate;
	motor->rotate = &rotate;
	motor->steps_per_revolution = steps_per_revolution;
	motor->step_delay = 60L * 1000L * 1000L / steps_per_revolution / speed;
	return motor;
}

void motor_rotate(struct MotorAttributes *motor, int steps_to_move)
{
	motor->rotate(motor, steps_to_move);
}

void motor_deactivate(struct MotorAttributes *motor)
{
	motor->deactivate(motor);
}

static void deactivate(struct MotorAttributes *motor)
{
	send_command(motor->info->motor_device, STOP, 0);
}

void rotate(struct MotorAttributes *motor, int steps_to_move)
{
	enum DriverCommands cmd = RIGHT;
	if (steps_to_move < 0)
		cmd = LEFT;
	send_command(motor->info->motor_device, cmd, steps_to_move);
}

void send_command(char *dev, int cmd, int sspeed)
{
	UartDriverPayload payload = { .command = cmd,
				      .x_steps = sspeed,
				      .y_steps = sspeed };
	motor_ioctl(dev, MOTOR_CMD, (int *)&payload);
	//	show_sage();
}

static void motor_ioctl(char *dev, int cmd, int *arg)
{
	int fd = open(dev, O_WRONLY);
	ioctl(fd, cmd, arg);
	close(fd);
}