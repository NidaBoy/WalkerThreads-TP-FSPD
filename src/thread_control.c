#include "thread_control.h"
#include <stdlib.h>

ThreadControl* thread_control_init(int num_threads, Board* board) {
    ThreadControl* controller = (ThreadControl*)malloc(sizeof(ThreadControl));
    if (!controller) {
        return NULL;
    }

    controller->num_threads = num_threads;
    controller->board = board;
    controller->entities = (ThreadEntity**)malloc(num_threads * sizeof(ThreadEntity*));
    
    if (!controller->entities) {
        free(controller);
        return NULL;
    }

    // Initialize entity pointers to NULL
    for (int i = 0; i < num_threads; i++) {
        controller->entities[i] = NULL;
    }

    return controller;
}

void thread_control_free(ThreadControl* controller) {
    if (!controller) {
        return;
    }

    // Free all thread entities
    if (controller->entities) {
        for (int i = 0; i < controller->num_threads; i++) {
            if (controller->entities[i]) {
                thread_entity_free(controller->entities[i]);
            }
        }
        free(controller->entities);
    }

    free(controller);
}

ThreadEntity* thread_control_create_entity(ThreadControl* controller, int index,
                                          int id, int group, int trajectory_length) {
    if (index < 0 || index >= controller->num_threads) {
        return NULL;
    }

    // Create the thread entity
    ThreadEntity* entity = thread_entity_init(id, group, trajectory_length, controller->board);
    if (!entity) {
        return NULL;
    }

    // Store in the controller
    controller->entities[index] = entity;

    return entity;
}

void thread_control_start_threads(ThreadControl* controller) {
    // Start all thread entities
    for (int i = 0; i < controller->num_threads; i++) {
        if (controller->entities[i]) {
            pthread_create(&controller->entities[i]->thread, NULL, 
                          thread_entity_run, controller->entities[i]);
        }
    }
}

void thread_control_join_threads(ThreadControl* controller) {
    for (int i = 0; i < controller->num_threads; i++) {
        if (controller->entities[i]) {
            pthread_join(controller->entities[i]->thread, NULL);
        }
    }
}