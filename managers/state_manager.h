//
// Created by viacheslav on 11.04.23.
//

#ifndef MOTORLAND_FORK_STATE_MANAGER_H
#define MOTORLAND_FORK_STATE_MANAGER_H

struct StateManager {
    ProtocolManager gpio_manager;
    ProtocolManager uart_manager;
    ProtocolManager spi_manager;

    Result (*move_motors)(StateManager* manager, MoveParams* params);
};

#endif //MOTORLAND_FORK_STATE_MANAGER_H
