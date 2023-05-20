#include "daemon_utils.h"

MotorCommand parseMotorCommand(const uint8_t* byteOrder, size_t size) {
    MotorCommand command;

    // Invalid byte order size
    // Handle error or return default command
    if (size != sizeof(MotorCommand) || byteOrder[4] != 0xFF) {
        MotorCommand nothing = {0, 0, 0, 0};
        return nothing;
    }

    // Extract the fields from the byte order
    command.motorIndex = byteOrder[0];
    command.numOfSteps = (byteOrder[1] << 8) | byteOrder[2];
    command.speed = byteOrder[3];
    command.checksum = byteOrder[4];

    return command;
}
