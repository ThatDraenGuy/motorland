#include "protocol_manager.h"

Result blocking_move(ProtocolManager *manager, RotationCommandWrapper wrapper) {
    for (size_t i = 0; i < wrapper.motors_count; ++i) {
        Result result = wrapper.motor_attributes[i].rotate(wrapper->steps[i]);
        if (result != SUCCESS) {
            return result;
        }
    }
    return SUCCESS;
}

//TODO: implement simultaneous movement