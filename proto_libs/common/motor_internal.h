#pragma once

#include "motor_api.h"

typedef struct ProtoSpecificInfo ProtoSpecificInfo;

typedef void (*rotate_fn)(struct MotorAttributes *motor, int steps_to_move);

typedef void (*deactivate_fn)(struct MotorAttributes *motor);
