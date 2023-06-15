#pragma once

#include <stdlib.h>

typedef struct MotorAttributes MotorAttributes;

typedef struct {
    int *pins;
    size_t pins_count;
} GpioAttributes;

typedef struct {
    int bus;
    int device;
    int mode;
    int max_speed_hz;
} SpiAttributes;

typedef struct {
    int bus;
    int address;
} I2cAttributes;

typedef struct {
    char *device_name;
    int baud_rate;
} UartAttributes;

typedef union {
    GpioAttributes gpio;
    SpiAttributes spi;
    I2cAttributes i2c;
    UartAttributes uart;
} SpecialMotorAttributes;

typedef struct {
    char *motor_name;
    //int motor_type;
    int steps_per_revolution;
    int steps_rev_per_minute;
} KeenMotorAttributes;

/*typedef struct {
    //int motor_type;
    KeenMotorAttributes keen_attributes;
    SpecialMotorAttributes special_attributes;
} Config;*/

typedef struct {
    char *motor_name;
    //int motor_type;
    int steps_per_revolution;
    int steps_rev_per_minute;

    GpioAttributes gpio_attributes;
    UartAttributes uart_attributes;

} Config;


typedef struct {
    Config *configs;
    size_t configs_count;
} ConfigWrapper;

typedef struct {
    MotorAttributes *attributes;
    size_t motors_count;
} MotorsWrapper;

/*typedef struct {
  char * motor_name;
  MotorConnection connection;
} Config;*/