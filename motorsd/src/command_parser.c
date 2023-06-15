#include "daemon_utils.h"
#include <stdio.h>

MotorCommand parse_motor_command(const uint8_t* command_byte_order, size_t size) {
    MotorCommand command;
  MotorCommand nothing = {0, 0, 0, 0};

  // Invalid byte order size
    // Handle error or return default command
    if (size != sizeof(MotorCommand)) {
        return nothing;
    }
    command = *(MotorCommand *)command_byte_order;
    if (command.checksum != 0xFF) {
        printf("Wrong checksum");
        return nothing;
    }

    return command;
}
