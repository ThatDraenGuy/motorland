#ifndef MOTORS_INTERNAL_H
#define MOTORS_INTERNAL_H

//TODO: do we need this import?
#include "api.h"
#include "stdint.h"

typedef enum {
    BACKWARD = 0,
    FORWARD = 1
} Direction;

/* Contains mutable state for single motor */
typedef struct {
    Direction direction;
    uint16_t current_steps;
    uint16_t max_steps;
    uint16_t min_steps;
    uint32_t step_delay_us;
    uint32_t last_step_time_us;

    // TODO: we should reguaraly update these fields if they are present here.
    /* bool max_triggered; */
    /* bool min_triggered; */
} MotorState;


typedef MovementResult (* motor_move_fn)(MotorState* state, uint16_t steps);
typedef MovementResult (* motor_deactivate_fn)(MotorAttributes* attributes);

/* Contains attributes of a single motor.
* Filled at the initialization phase.
*/
struct MotorAttributes {
    motor_move_fn move;
    motor_deactivate_fn deactivate;

    //TODO: do we need a step angle and how it can be used?
    uint16_t number_of_steps;
    bool invert;
    MotorConnection connection;
    MotorState state;
};

#endif /* MOTORS_INTERNAL_H */