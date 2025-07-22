#ifndef BOARD_H
#define BOARD_H

#include <pthread.h>

/*
 * Here is where the sicronizations problems are truly solved
*/


/**
 * Represents a cell in the board
 */
typedef struct {
    int occupied;              // Whether this cell is occupied
    int group;                 // Group ID of the thread occupying this cell
    int thread_id;             // ID of the thread occupying this cell
    pthread_mutex_t mutex;     // Mutex for exclusive access to the cell
    pthread_cond_t condition;  // Condition variable for waiting on the cell
} Cell;

/**
 * Represents the game board
 */
typedef struct {
    int size;                  // Size of the board (NxN)
    Cell** cells;              // 2D array of cells
} Board;

/**
 * Initialize the board
 */
Board* board_init(int size);

/**
 * Free the board and associated resources
 */
void board_free(Board* board);

/**
 * Try to enter a position on the board
 */
void board_enter(Board* board, int x, int y, int thread_id, int group_id);

/**
 * Exit a position on the board
 */
void board_exit(Board* board, int x, int y);

#endif /* BOARD_H */