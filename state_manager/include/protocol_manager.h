#pragma once

// TODO: add verdicts
typedef enum {
    SUCCESS = 0,
    MAX_REACHED,
    MIN_REACHED,
    COMMUNICATION_ERROR
} MovementResult;

typedef struct {
    Motor *motors;
    // TODO: decide if whe need copy of MotorAttributes. It can be useful to relate motors to their queues
    Queue *motor_queues;
    /*
    We use unix-threads to move motors simultaneously. New commands will be added to
     motor-queues and the corresponding thread will be notified.
     Blocking movement can be also supported by adding mutex lock
     */
    pthread_t *motor_threads;
    move_motors_fn move_motors;

} ProtocolManager;

typedef MovementResult *(*move_motors_fn)(ProtocolManager *manager, RotationCommandWrapper wrapper);

typedef struct {
    // TODO: what do we need here
} RotationCommandWrapper;