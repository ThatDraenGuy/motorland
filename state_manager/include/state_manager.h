#include "protocol_manager.h"

#pragma once

typedef struct {
    ProtocolManager gpio_manager;
    ProtocolManager uart_manager;
    ProtocolManager spi_manager;

    move_motors_fn move_motors;
} StateManager;


StateManager state_manager_init(MotorsWrapper wrapper);

void state_manager_add_movements(StateManager *manager, MotorsMovements movements);

/*
As interrupts in user-space are not supported we use a separate threat
 that checks if the motor has finished moving and passes next command from the motor's queue
 */

void state_manager_move_motors(StateManager *manager);