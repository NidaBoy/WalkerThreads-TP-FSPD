#ifndef THREAD_CONTROL_H
#define THREAD_CONTROL_H

#include "thread_entity.h"

/**
 * Thread controller structure
 */
typedef struct {
    int num_threads;         // Number of threads
    ThreadEntity** entities; // Array of thread entities
    Board* board;            // Reference to the board
} ThreadControl;

/**
 * Initialize the thread controller
 */
ThreadControl* thread_control_init(int num_threads, Board* board);

/**
 * Free the thread controller and associated resources
 */
void thread_control_free(ThreadControl* controller);

/**
 * Create a thread entity and add it to the controller
 */
ThreadEntity* thread_control_create_entity(ThreadControl* controller, int index, 
                                          int id, int group, int trajectory_length);

/**
 * Start all threads in the controller
 */
void thread_control_start_threads(ThreadControl* controller);

/**
 * Wait for all threads to complete
 */
void thread_control_join_threads(ThreadControl* controller);

#endif /* THREAD_CONTROL_H */