#pragma once

#include "motor_api.h"

typedef struct ProtoSpecificInfo ProtoSpecificInfo;

typedef void (*rotate_fn)(struct MotorAttributes *motor, int steps_to_move);

typedef void (*deactivate_fn)(struct MotorAttributes *motor);

struct MotorAttributes {
	rotate_fn rotate;
	deactivate_fn deactivate;
	struct ProtoSpecificInfo *info;
	uint32_t current_steps;
	uint32_t steps_per_revolution;
	uint16_t step_delay;
};
