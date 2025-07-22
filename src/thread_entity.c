#include "thread_entity.h"
#include <stdlib.h>

// External function declaration
extern void passa_tempo(int tid, int x, int y, int decimos);

ThreadEntity* thread_entity_init(int id, int group, int trajectory_length, Board* board) {
    ThreadEntity* entity = (ThreadEntity*)malloc(sizeof(ThreadEntity));
    if (!entity) {
        return NULL;
    }

    entity->id = id;
    entity->group = group;
    entity->board = board;
    entity->trajectory.length = trajectory_length;
    entity->trajectory.positions = (Position*)malloc(trajectory_length * sizeof(Position));
    
    if (!entity->trajectory.positions) {
        free(entity);
        return NULL;
    }

    return entity;
}

void thread_entity_free(ThreadEntity* entity) {
    if (!entity) {
        return;
    }

    if (entity->trajectory.positions) {
        free(entity->trajectory.positions);
    }
    
    free(entity);
}

void thread_entity_add_position(ThreadEntity* entity, int pos_index, int x, int y, int time) {
    
    if (pos_index < entity->trajectory.length) {
        entity->trajectory.positions[pos_index].x = x;
        entity->trajectory.positions[pos_index].y = y;
        entity->trajectory.positions[pos_index].time = time;
    }
}

void* thread_entity_run(void* arg) {
    ThreadEntity* entity = (ThreadEntity*)arg;
    
    if (entity->trajectory.length <= 0) {
        return NULL;
    }
    
    // Get the first position
    Position first_pos = entity->trajectory.positions[0];
    
    // Enter the first position (should always be possible as per specifications)
    board_enter(entity->board, first_pos.x, first_pos.y, entity->id, entity->group);
    
    // Spend time in the first position
    passa_tempo(entity->id, first_pos.x, first_pos.y, first_pos.time);
    
    // Process the rest of the trajectory
    for (int i = 1; i < entity->trajectory.length; i++) {
        Position current_pos = entity->trajectory.positions[i];
        Position prev_pos = entity->trajectory.positions[i-1];
        
        // Try to enter the next position
        board_enter(entity->board, current_pos.x, current_pos.y, entity->id, entity->group);
        
        // Once successfully entered, exit the previous position
        board_exit(entity->board, prev_pos.x, prev_pos.y);
        
        // Spend time in the current position
        passa_tempo(entity->id, current_pos.x, current_pos.y, current_pos.time);
    }
    
    // Exit the last position after completing the trajectory
    Position last_pos = entity->trajectory.positions[entity->trajectory.length - 1];
    board_exit(entity->board, last_pos.x, last_pos.y);
    
    return NULL;
}