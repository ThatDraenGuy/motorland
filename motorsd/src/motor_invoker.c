#include "motor_invoker.h"
#include <string.h>

// вот не подключается
#include "../../proto_libs/gpio_lib/include/motor_api.h"
#include "../../proto_libs/gpio_lib/include/motor_internal.h"

void process_motor_command(MotorCommand command) {
    printf("\nindex: %d\nspeed: %d\nsteps: %d\n", command.motorIndex, command.speed, command.numOfSteps);
    // StateManager manager
    // transform command to MoveParams
    // manager.move_command(manager, params)

    //stub
    ConnectionAttributes connection;
    GpioConnectionData vertical_motor_conf = { .pins = { 58, 60, 59, 61 } };
    GpioConnectionData horizontal_motor_conf = { .pins = { 31, 33, 32,
							   34 } };
    GpioConnectionData conf[2];
    conf[0] = vertical_motor_conf;
    conf[1] = horizontal_motor_conf;

    int steps_per_revolution = 2038;
    memcpy(connection.gpio.pins, &conf[command.motorIndex].pins,
	   sizeof(conf[command.motorIndex].pins));
    struct MotorAttributes *motor =
	    motor_setup(connection, steps_per_revolution, command.speed);
    if (!motor) {
		fprintf(stderr, "Failed to initialize motor: %d\n",
			command.motorIndex);
		return;
    }
    motor_rotate(motor, command.numOfSteps);

    motor_deactivate(motor);
}
