#include <malloc.h>
#include "internal.h"

void motor_exit(MotorAttributes* attributes) {
    // motor-specific way of deactivation (turn off gpio/ send stop commands etc.)
    attributes->deactivate(attributes);
    free(attributes);
}


MovementResult motor_move(MotorAttributes* attributes, int steps) {
    // TODO: do we need common checks for all protocols like max/min positions here?
    return attributes->proto_specific_move(&attributes->state, steps);
}


void motor_set_max_enstop(MotorAttributes* attributes, int max_pin) {
    // TODO: create new struct in MotorAttributes with physical pins
    // these pins should be checked on each step.
}


void motor_set_min_enstop(MotorAttributes* attributes, int min_pin) {}


void motor_set_min_softlimit(MotorAttributes* attributes, int min_steps) {
    attributes->state.min_steps = min_steps;
}

void motor_set_max_softlimit(MotorAttributes* attributes, int max_steps) {
    attributes->state.max_steps = max_steps;
}

void motor_invert_direction(MotorAttributes* attributes, bool invert) {
    attributes->invert = invert;
}

