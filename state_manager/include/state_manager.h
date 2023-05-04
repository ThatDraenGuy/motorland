#pragma once

typedef struct {
    ProtocolManager gpio_manager;
    ProtocolManager uart_manager;
    ProtocolManager spi_manager;

    Result (*move_motors)(StateManager* manager, MoveParams* params);
} stateManager;
