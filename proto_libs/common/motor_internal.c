#include "motor_internal.h"

struct MotorAttributes {
	rotate_fn rotate;
	deactivate_fn deactivate;
	struct ProtoSpecificInfo *info;
	uint32_t current_steps;
	uint32_t steps_per_revolution;
	uint16_t step_delay;
};
