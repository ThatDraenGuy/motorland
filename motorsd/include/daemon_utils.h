#ifndef MOTOR_COMMAND_PARSER_H
#define MOTOR_COMMAND_PARSER_H

#include <stdint.h>
#include <stddef.h>

typedef struct {
    uint8_t motorIndex;
    uint16_t numOfSteps;
    uint8_t speed;
} MotorCommand;

MotorCommand parseMotorCommand(const uint8_t* byteOrder, size_t size);

#endif  // MOTOR_COMMAND_PARSER_H
