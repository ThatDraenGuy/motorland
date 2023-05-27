#include "daemon_utils.h"

MotorCommand parse_motor_command(const uint8_t* command_byte_order, size_t size) {
    MotorCommand command;

    // Invalid byte order size
    // Handle error or return default command
    if (size != sizeof(MotorCommand) || command_byte_order[4] != 0xFF) {
        MotorCommand nothing = {0, 0, 0, 0};
        return nothing;
    }

    // Extract the fields from the byte order
    command.motorIndex = command_byte_order[0];
    command.numOfSteps = (command_byte_order[1] << 8) | command_byte_order[2];
    command.speed = command_byte_order[3];
    command.checksum = command_byte_order[4];

    return command;
}
