#include "malloc.h"
#include "internal.h"

MotorState motor_state_init();

MovementResult motor_gpio_move(MotorState* state, int steps);

void motor_gpio_step(uint32_t step);

void setOutputPins(int i);

MotorAttributes* motor_init(int number_of_steps, MotorConnection connection) {
    MotorAttributes* attributes = (MotorAttributes*) malloc(sizeof(MotorAttributes));
    if (!attributes) {
        // TODO: create unified way to handle errors.
        return NULL;
    }

    attributes->proto_specific_move = &motor_gpio_move;
    attributes->number_of_steps = number_of_steps;
    attributes->invert = false;
    attributes->connection = connection;
    attributes->state = motor_state_init();
}


MovementResult motor_gpio_move(MotorState* state, int steps) {
    if (state->direction == FORWARD && state->current_steps >= state->max_steps) {
        return MAX_REACHED;
    }
    if (state->direction == BACKWARD && state->current_steps <= state->min_steps) {
        return MIN_REACHED;
    }

    for (uint32_t i = 0; i < steps; i++) {
        motor_gpio_step(i);
        // sleep until the phase time passes
    }
    return SUCCESS;
}

MotorState motor_state_init() {
    MotorState state;
    state.current_steps = 0;

    return state;
}

void motor_gpio_step(uint32_t step) {
    switch (step & 0x3) {
        case 0:    // 1010
            setOutputPins(0b0101);
            break;
        case 1:    // 0110
            setOutputPins(0b0110);
            break;
        case 2:    //0101
            setOutputPins(0b1010);
            break;
        case 3:    //1001
            setOutputPins(0b1001);
            break;
    }
}

void setOutputPins(int i) {
    // setting multiple pins
}

