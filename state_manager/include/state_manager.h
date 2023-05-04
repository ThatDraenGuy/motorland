#pragma once

typedef struct {
    ProtocolManager gpio_manager;
    ProtocolManager uart_manager;
    ProtocolManager spi_manager;

<<<<<<< HEAD
    Result (*move_motors)(StateManager* manager, MoveParams* params);
} stateManager;
=======
    move_motors_fn move_motors;
} StateManager;
>>>>>>> 69ea2e1 (feat: fixed issues on managers)
