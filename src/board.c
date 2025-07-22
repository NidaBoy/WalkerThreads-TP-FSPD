#include "board.h"
#include <stdlib.h>

Board* board_init(int size) {
    Board* board = (Board*)malloc(sizeof(Board));
    if (!board) {
        return NULL;
    }

    board->size = size;
    
    // Allocate cells for the board
    board->cells = (Cell**)malloc(size * sizeof(Cell*));
    if (!board->cells) {
        free(board);
        return NULL;
    }

    for (int i = 0; i < size; i++) {
        board->cells[i] = (Cell*)malloc(size * sizeof(Cell));
        if (!board->cells[i]) {
            // Clean up if allocation fails
            for (int j = 0; j < i; j++) {
                free(board->cells[j]);
            }
            free(board->cells);
            free(board);
            return NULL;
        }

        // Initialize each cell
        for (int j = 0; j < size; j++) {
            board->cells[i][j].occupied = 0;
            board->cells[i][j].group = -1;
            board->cells[i][j].thread_id = -1;
            pthread_mutex_init(&board->cells[i][j].mutex, NULL);
            pthread_cond_init(&board->cells[i][j].condition, NULL);
        }
    }

    return board;
}

void board_free(Board* board) {
    if (!board) {
        return;
    }

    // Free all cells and destroy mutexes and condition variables
    for (int i = 0; i < board->size; i++) {
        if (board->cells[i]) {
            for (int j = 0; j < board->size; j++) {
                pthread_mutex_destroy(&board->cells[i][j].mutex);
                pthread_cond_destroy(&board->cells[i][j].condition);
            }
            free(board->cells[i]);
        }
    }

    free(board->cells);
    free(board);
}

void board_enter(Board* board, int x, int y, int thread_id, int group_id) {
    // Lock the mutex for the cell
    pthread_mutex_lock(&board->cells[x][y].mutex);

    // Wait until the cell is unoccupied or occupied by a thread of the same group
    while (board->cells[x][y].occupied && board->cells[x][y].group != group_id) {
        pthread_cond_wait(&board->cells[x][y].condition, &board->cells[x][y].mutex);
    }

    // Enter the cell
    board->cells[x][y].occupied = 1;
    board->cells[x][y].group = group_id;
    board->cells[x][y].thread_id = thread_id;

    // Unlock the mutex
    pthread_mutex_unlock(&board->cells[x][y].mutex);
}

void board_exit(Board* board, int x, int y) {
    // Lock the mutex for the cell
    pthread_mutex_lock(&board->cells[x][y].mutex);

    // Mark cell as unoccupied
    board->cells[x][y].occupied = 0;
    board->cells[x][y].group = -1;
    board->cells[x][y].thread_id = -1;

    // Signal all waiting threads that the cell is now available
    pthread_cond_broadcast(&board->cells[x][y].condition);

    // Unlock the mutex
    pthread_mutex_unlock(&board->cells[x][y].mutex);
}