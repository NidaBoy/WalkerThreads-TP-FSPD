#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "thread_control.h"
#include "thread_entity.h"

int main() {
    int board_size, num_threads;
    
    // Read board size and number of threads
    scanf("%d %d", &board_size, &num_threads);
    
    // Initialize the board
    Board* board = board_init(board_size);
    if (!board) {
        fprintf(stderr, "Failed to initialize board\n");
        return 1;
    }
    
    // Initialize thread controller
    ThreadControl* controller = thread_control_init(num_threads, board);
    if (!controller) {
        fprintf(stderr, "Failed to initialize thread controller\n");
        board_free(board);
        return 1;
    }
    
    // Read thread information and create thread entities
    for (int i = 0; i < num_threads; i++) {
        int thread_id, group_id, traj_length;
        
        // Read thread ID, group ID, and trajectory length
        scanf("%d %d %d", &thread_id, &group_id, &traj_length);
        
        // Create thread entity
        ThreadEntity* entity = thread_control_create_entity(controller, i, thread_id, group_id, traj_length);
        if (!entity) {
            fprintf(stderr, "Failed to create thread entity %d\n", i);
            continue;
        }
        
        // Read trajectory positions
        for (int j = 0; j < traj_length; j++) {
            int x, y, time;
            scanf("%d %d %d", &x, &y, &time);
            thread_entity_add_position(entity, j, x, y, time);
        }
    }
    
    // Start all threads
    thread_control_start_threads(controller);
    
    // Wait for all threads to complete
    thread_control_join_threads(controller);
    
    // Clean up
    thread_control_free(controller);
    board_free(board);
    
    return 0;
}