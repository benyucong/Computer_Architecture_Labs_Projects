#ifndef _SNK_STATE_H
#define _SNK_STATE_H

#include <stdbool.h>
#include <stdio.h>

typedef struct snake_t
{
    unsigned int tail_x; // The x-coordinate (column) of the snake's tail.
    unsigned int tail_y; // The y-coordinate (row) of the snake's tail.
    unsigned int head_x; // The x-coordinate (column) of the snake's head.
    unsigned int head_y; // The y-coordinate (row) of the snake's head.

    bool live; // true if the snake is alive, and false if the snake is dead.
} snake_t;

typedef struct game_state_t
{
    unsigned int x_size; // Width of the game board.
    unsigned int y_size; // Height of the game board.

    /* The game board in memory.
    Each element of the board array is a char* pointer to a character array containing a row of the map. */
    char **board;

    unsigned int num_snakes; // The number of snakes on the board.
    snake_t *snakes;         // An array of snake structs.
} game_state_t;

game_state_t *create_default_state();

void free_state(game_state_t *state);

void print_board(game_state_t *state, FILE *fp);

void save_board(game_state_t *state, char *filename);

void update_state(game_state_t *state, int (*add_food)(game_state_t *state));

game_state_t *initialize_snakes(game_state_t *state);

game_state_t *load_board(char *filename);

#endif
