#include "motor_invoker.h"


void process_motor_command(MotorCommand command) {
    printf("\nindex: %d\nspeed: %d\nsteps: %d\n", command.motorIndex, command.speed, command.numOfSteps);
    // StateManager manager
    // transform command to MoveParams
    // manager.move_command(manager, params)
}
