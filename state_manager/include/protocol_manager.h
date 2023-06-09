#pragma once

#include "stdlib.h"

// TODO: add verdicts
typedef enum {
    SUCCESS = 0,
    MAX_REACHED,
    MIN_REACHED,
    COMMUNICATION_ERROR
} MovementResult;

typedef struct {
    MotorAttributes *attributes;
    int *steps_to_move;
    size_t motors_count;
} MotorsMovements;

typedef struct {
    int *steps_to_move;
} MotorQueue;

typedef struct {
    MotorAttributes *attributes;
    MotorQueue *motor_queues;
    pthread_t *motor_threads;
    move_motors_fn move_motors;

} ProtocolManager;

typedef MovementResult *(*move_motors_fn)(ProtocolManager *manager, MotorsMovements wrapper);
