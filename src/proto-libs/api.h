#ifndef MOTORS_API_H
#define MOTORS_API_H

#include "stdbool.h"
#include "stdint.h"

/* Immutable attributes of a single motor connected via one of the supported protocols */
typedef struct MotorAttributes MotorAttributes;

/* Represents how the stepper is connected to the main board */
typedef enum {
    GPIO = 0,
    SPI,
    I2C
} CommunicationType;

typedef struct {
    uint8_t pins[4];
} GpioConnectionData;

typedef struct {
    char* spi_dev_name;
} SpiConnectionData;

typedef struct {
    char* i2c_dev_name;
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
MotorAttributes* motor_init(uint16_t number_of_steps, MotorConnection connection);

/* Deinitilizes motor and destroyes attributes structure */
void motor_exit(MotorAttributes* attributes);

MovementResult motor_move(MotorAttributes* attributes, uint16_t steps);

/* how much time to wait between commands. Useful when commnuicating via i2c/spi */
void motor_set_command_delay(MotorAttributes* attributes, uint32_t delay_ms);

/* set gpio pins of max/min physical endstops, which trigger when motor reaches the limit position */
void motor_set_max_enstop(MotorAttributes* attributes, uint16_t max_pin);

void motor_set_min_enstop(MotorAttributes* attributes, uint16_t min_pin);

// TODO: decide where to start counting the steps. From the left or from center?
/* sets max/min positions for the motor */
void motor_set_min_softlimit(MotorAttributes* attributes, uint16_t min_steps);

void motor_set_max_softlimit(MotorAttributes* attributes, uint16_t max_steps);

/* inverts default rotation direction */
void motor_set_axis_inversion(MotorAttributes* attributes, bool invert);


/* TODO: decide how the upper layer can get the info about motor state. Is this method sutable here? */
// MotorStatus get_current_status(MotorAttributes* attributes);

// TODO: do we need preset movements - moving to specific position?

#endif /* MOTORS_API_H */