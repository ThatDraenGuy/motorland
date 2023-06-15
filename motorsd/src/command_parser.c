#include "daemon_utils.h"
#include <stdio.h>

void print_parsed_command(MotorCommand command) {
	printf("Command Info\n");
	printf("Motor index %d\n", command.motorIndex);
	printf("Motor steps %d\n", command.numOfSteps);
	printf("Motor speed %d\n", command.speed);
	printf("Command checksum %d\n", command.checksum);
}

MotorCommand parse_motor_command(const uint8_t* command_byte_order, size_t size) {
    MotorCommand command;

    // Invalid byte order size
    // Handle error or return default command
    if (size != sizeof(MotorCommand) || command_byte_order[4] != 0xFF) {
        MotorCommand nothing = {0, 0, 0, 0};
	printf("Command parse invalid");
        return nothing;
    }

    // Extract the fields from the byte order
    command.motorIndex = command_byte_order[0];
    command.numOfSteps = (command_byte_order[1] << 8) | command_byte_order[2];
    command.speed = command_byte_order[3];
    command.checksum = command_byte_order[4];

    print_parsed_command(command);

    return command;
}
