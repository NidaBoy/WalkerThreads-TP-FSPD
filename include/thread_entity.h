#ifndef THREAD_ENTITY_H
#define THREAD_ENTITY_H

#include "board.h"

/**
 * Represents a position in the trajectory
 */
typedef struct {
    int x;          // X coordinate
    int y;          // Y coordinate
    int time;       // Time to spend at this position (in tenths of a second)
} Position;

/**
 * Represents a trajectory for a thread entity
 */
typedef struct {
    int length;         // Number of positions in the trajectory
    Position* positions; // Array of positions
} Trajectory;

/**
 * Represents an entity (thread) in the system
 */
typedef struct {
    int id;             // Thread ID
    int group;          // Group ID
    Trajectory trajectory; // Trajectory to follow
    Board* board;       // Reference to the board
    pthread_t thread;   // Thread handle
} ThreadEntity;

/**
 * Initialize a thread entity
 */
ThreadEntity* thread_entity_init(int id, int group, int trajectory_length, Board* board);

/**
 * Free a thread entity and associated resources
 */
void thread_entity_free(ThreadEntity* entity);

/**
 * Add a position to the trajectory of a thread entity
 */
void thread_entity_add_position(ThreadEntity* entity, int pos_index, int x, int y, int time);

/**
 * Thread function for a thread entity
 */
void* thread_entity_run(void* arg);

#endif /* THREAD_ENTITY_H */