#pragma once

typedef struct {
    ProtocolManager gpio_manager;
    ProtocolManager uart_manager;
    ProtocolManager spi_manager;

    move_motors_fn move_motors;
} StateManager;
