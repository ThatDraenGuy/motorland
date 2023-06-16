#pragma once

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t motorIndex;
    int16_t numOfSteps;
    uint8_t speed;
    uint8_t checksum;
} MotorCommand;

MotorCommand parse_motor_command(const uint8_t* command_byte_order, size_t size);
