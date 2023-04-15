//
// Created by viacheslav on 11.04.23.
//

#ifndef MOTORLAND_FORK_PROTOCOL_MANAGER_H
#define MOTORLAND_FORK_PROTOCOL_MANAGER_H

// TODO: add verdicts
typedef enum {
    SUCCESS = 0,
    MAX_REACHED,
    MIN_REACHED,
    COMMUNICATION_ERROR
} MovementResult;

struct ProtocolManager {
    Motor *motors;
    Queue *motor_queues;
    /*
    We use unix-threads to move motors simultaneously. New commands will be added to
     motor-queues and the corresponding thread will be notified.
     Blocking movement can be also supported by adding mutex lock
     */
    pthread_t *motor_threads;
    MovementResult (*move_motors)(ProtocolManager *manager, MotorAttributes* attributes);
};


#endif //MOTORLAND_FORK_PROTOCOL_MANAGER_H
