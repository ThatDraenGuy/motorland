#pragma once

#include <stdint.h>
#include "stdbool.h"

/* Immutable attributes of a single motor connected via one of the supported protocols */
typedef struct MotorAttributes MotorAttributes;

struct GpioInfo;

/* Represents how the stepper is connected to the main board */
typedef enum { GPIO = 0, SPI, I2C } CommunicationType;

typedef struct {
	int pins[4];
} GpioConnectionData;

typedef struct {
	char *spi_dev_name;
} SpiConnectionData;

typedef struct {
	char *i2c_dev_name;
} I2cConnectionData;

/* Stores data required for one of the supported protocols */
typedef union {
	GpioConnectionData gpio;
	SpiConnectionData spi;
	I2cConnectionData i2c;
} ConnectionAttributes;

/* Stores data required to communicate with a motor/extension-board */
typedef struct {
	CommunicationType communicationType;
	ConnectionAttributes connectionAttributes;
} MotorConnection;

/* Represent the status of movement invocation */
typedef enum {
	SUCCESS = 0,
	MAX_REACHED,
	MIN_REACHED,
	COMMUNICATION_ERROR
} MovementResult;

/* Creates an opaque structure with motor attributes. */
struct MotorAttributes *motor_setup(uint32_t speed);

void motor_rotate(struct MotorAttributes *motor, int steps_to_move);

/* De-initializes motor and destroys attributes structure */
void motor_deactivate(struct MotorAttributes *motor);

/* how much time to wait between commands. Useful when commnuicating via i2c/spi */
void motor_set_command_delay(MotorAttributes *attributes, int delay_ms);

/* set gpio pins of max/min physical endstops, which trigger when motor reaches the limit position */
void motor_set_max_enstop(MotorAttributes *attributes, int max_pin);

void motor_set_min_enstop(MotorAttributes *attributes, int min_pin);

// TODO: decide where to start counting the steps. From the left or from center?
/* sets max/min positions for the motor */
void motor_set_min_softlimit(MotorAttributes *attributes, int min_steps);

void motor_set_max_softlimit(MotorAttributes *attributes, int max_steps);

/* inverts default rotation direction */
void motor_invert_direction(MotorAttributes *attributes, bool invert);
