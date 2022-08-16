#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "snake_utils.h"
#include "state.h"

/* Helper function definitions */
static char get_board_at(game_state_t *state, int x, int y);

static void set_board_at(game_state_t *state, int x, int y, char ch);

static bool is_tail(char c);

static bool is_snake(char c);

static char body_to_tail(char c);

static int incr_x(char c);

static int incr_y(char c);

static void find_head(game_state_t *state, int snum);

static char next_square(game_state_t *state, int snum);

static void update_tail(game_state_t *state, int snum);

static void update_head(game_state_t *state, int snum);

/* Helper function to get a character from the board (already implemented for you). */
static char get_board_at(game_state_t *state, int x, int y)
{
    return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for you). */
static void set_board_at(game_state_t *state, int x, int y, char ch)
{
    state->board[y][x] = ch;
}

/* Task 1 */
game_state_t *create_default_state()
{
    // TODO: Implement this function.
    game_state_t *default_state = (game_state_t *)malloc(sizeof(game_state_t));
    default_state->x_size = 14; // Width, num of columns
    default_state->y_size = 10; // Height, num of rows
    default_state->board = (char **)malloc(default_state->y_size * sizeof(char *));
    const char *str1 = "##############";
    const char *str2 = "#            #";
    for (int i = 0; i < default_state->y_size; ++i)
    {
        default_state->board[i] = (char *)malloc(default_state->x_size * sizeof(char));
        if (i == 0 || i == default_state->y_size - 1)
        {
            strcpy(default_state->board[i], str1);
        }
        else
        {
            strcpy(default_state->board[i], str2);
        }
    }
    default_state->board[2][9] = '*';
    default_state->board[4][4] = 'd';
    default_state->board[4][5] = '>';
    default_state->num_snakes = 1;
    default_state->snakes = (snake_t *)malloc(default_state->num_snakes * sizeof(snake_t));
    default_state->snakes[0].tail_x = 4;
    default_state->snakes[0].tail_y = 4;
    default_state->snakes[0].head_x = 5;
    default_state->snakes[0].head_y = 4;
    default_state->snakes[0].live = true;
    return default_state;
}

/* Task 2 */
void free_state(game_state_t *state)
{
    // TODO: Implement this function.
    free(state->snakes);
    for (int i = 0; i < state->y_size; ++i)
    {
        free(state->board[i]);
    }
    free(state->board);
    free(state);
    return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp)
{
    // TODO: Implement this function.
    for (int i = 0; i < state->y_size; ++i)
    {
        fprintf(fp, "%s\n", state->board[i]);
    }
    return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t *state, char *filename)
{
    FILE *f = fopen(filename, "w");
    print_board(state, f);
    fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c)
{
    // TODO: Implement this function.
    if (c == 'w' || c == 'a' || c == 's' || c == 'd')
    {
        return true;
    }
    return false;
}

static bool is_snake(char c)
{
    // TODO: Implement this function.
    if (is_tail(c) || c == '^' || c == '<' || c == '>' || c == 'v' || c == 'x')
    {
        return true;
    }
    return false;
}

static char body_to_tail(char c)
{
    // TODO: Implement this function.
    if (c == '^')
        return 'w';
    else if (c == '<')
        return 'a';
    else if (c == '>')
        return 'd';
    else if (c == 'v')
        return 's';
    return '0';
}

static int incr_x(char c)
{
    // TODO: Implement this function.
    if (c == '>' || c == 'd')
        return 1;
    else if (c == '<' || c == 'a')
        return -1;
    else
        return 0;
}

static int incr_y(char c)
{
    // TODO: Implement this function.
    if (c == 'v' || c == 's')
        return 1;
    else if (c == '^' || c == 'w')
        return -1;
    else
        return 0;
}

/* Task 4.2 */
static char next_square(game_state_t *state, int snum)
{
    // TODO: Implement this function.
    snake_t snk = state->snakes[snum];
    char head = get_board_at(state, snk.head_x, snk.head_y);
    int new_head_x = snk.head_x + incr_x(head);
    int new_head_y = snk.head_y + incr_y(head);
    return get_board_at(state, new_head_x, new_head_y);
}

/* Task 4.3 */
static void update_head(game_state_t *state, int snum)
{
    // TODO: Implement this function.
    snake_t *snk = &state->snakes[snum];
    char head = get_board_at(state, snk->head_x, snk->head_y);
    snk->head_x += incr_x(head);
    snk->head_y += incr_y(head);
    set_board_at(state, snk->head_x, snk->head_y, head);
    return;
}

/* Task 4.4 */
static void update_tail(game_state_t *state, int snum)
{
    // TODO: Implement this function.
    snake_t *snk = &state->snakes[snum];
    char tail = get_board_at(state, snk->tail_x, snk->tail_y);
    set_board_at(state, snk->tail_x, snk->tail_y, ' ');
    snk->tail_x += incr_x(tail);
    snk->tail_y += incr_y(tail);
    char new_tail = body_to_tail(get_board_at(state, snk->tail_x, snk->tail_y));
    set_board_at(state, snk->tail_x, snk->tail_y, new_tail);
    return;
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state))
{
    // TODO: Implement this function.
    int size = state->num_snakes;
    for (int i = 0; i < size; i++)
    {
        if (!state->snakes[i].live)
            continue;
        if (next_square(state, i) == '#' || is_snake(next_square(state, i)))
        {
            state->snakes[i].live = false;
            set_board_at(state, state->snakes[i].head_x, state->snakes[i].head_y, 'x');
        }
        else if (next_square(state, i) == '*')
        {
            update_head(state, i);
            add_food(state);
        }
        else
        {
            update_head(state, i);
            update_tail(state, i);
        }
    }
    return;
}

/* Task 5 */
game_state_t *load_board(char *filename)
{
    // TODO: Implement this function.
    game_state_t *state = (game_state_t *)malloc(sizeof(game_state_t));
    FILE *fp;
    fp = fopen(filename, "rt");
    int N = 10000;
    char file[N];
    for (int k = 0; k < N; ++k)
    {
        char ch = fgetc(fp);
        if (feof(fp))
        {
            file[k] = '\0';
            break;
        }
        else
        {
            file[k] = ch;
        }
    }
    fclose(fp);
    size_t size = strlen(file);
    int width = 0, height;
    for (int i = 0; i < size; ++i)
    {
        if (file[i] == '#')
            ++width;
        else
            break;
    }
    height = (int)size / (width + 1); // consider '\n'
    state->x_size = width;
    state->y_size = height;
    state->board = (char **)malloc(state->y_size * sizeof(char *));
    for (int i = 0; i < state->y_size; ++i)
    {
        state->board[i] = (char *)malloc(state->x_size * sizeof(char));
        char tmp[state->x_size];
        for (int j = 0; j < state->x_size; ++j)
        {
            int index = i * (width + 1) + j;
            tmp[j] = file[index];
        }
        strcpy(state->board[i], tmp);
    }
    state->num_snakes = 0;
    state->snakes = (snake_t *)malloc(sizeof(snake_t));
    return state;
}

/* Task 6.1 */
static void find_head(game_state_t *state, int snum)
{
    // TODO: Implement this function.
    snake_t *snk = &state->snakes[snum];
    snk->head_x = snk->tail_x;
    snk->head_y = snk->tail_y;
    char ch;
    while (is_snake(next_square(state, snum)))
    {
        ch = get_board_at(state, snk->head_x, snk->head_y);
        snk->head_x += incr_x(ch);
        snk->head_y += incr_y(ch);
    }
    return;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state)
{
    // TODO: Implement this function.
    // Fill in num_snakes and create the snakes array.
    char ch;
    char head;
    state->num_snakes = 0;
    for (int i = 0; i < state->y_size; ++i)
    {
        for (int j = 0; j < state->x_size; ++j)
        {
            ch = get_board_at(state, j, i);
            if (is_tail(ch))
            {
                if (!state->num_snakes)
                {
                    snake_t *snk = &state->snakes[0];
                    snk->tail_x = j;
                    snk->tail_y = i;
                    find_head(state, 0);
                    head = get_board_at(state, snk->head_x, snk->head_y);
                    if (head == 'x')
                    {
                        snk->live = false;
                    }
                    else
                    {
                        snk->live = true;
                    }
                }
                else
                {
                    state->snakes = (snake_t *)realloc(state->snakes, (state->num_snakes + 1) * sizeof(snake_t));
                    snake_t *snk = &state->snakes[state->num_snakes];
                    snk->tail_x = j;
                    snk->tail_y = i;
                    find_head(state, state->num_snakes);
                    head = get_board_at(state, snk->head_x, snk->head_y);
                    if (head == 'x')
                    {
                        snk->live = false;
                    }
                    else
                    {
                        snk->live = true;
                    }
                }
                state->num_snakes++;
            }
        }
    }
    return state;
}
