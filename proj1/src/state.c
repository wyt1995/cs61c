#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"
#include "state.h"

#define DEFAULT_ROW_NUM 18
#define DEFAULT_COL_NUM 20

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static bool is_wall(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);
static void update_dead_snake(game_state_t* state, unsigned int snum);

/* Task 1 */
game_state_t* create_default_state() {
    game_state_t* game_state = (game_state_t *) malloc(sizeof(game_state_t));
    game_state->num_rows = DEFAULT_ROW_NUM;
    game_state->num_snakes = 1;
    game_state->board = (char **) malloc(DEFAULT_ROW_NUM * sizeof(char *));
    game_state->snakes = (snake_t *) malloc(sizeof(snake_t));

    // initial board state
    for (int i = 0; i < DEFAULT_ROW_NUM; i++) {
        game_state->board[i] = (char *) malloc((DEFAULT_COL_NUM + 1) * sizeof(char));
        game_state->board[i][0] = '#';
        for (int j = 1; j < DEFAULT_COL_NUM - 1; j++) {
            game_state->board[i][j] = ' ';
        }
        game_state->board[i][DEFAULT_COL_NUM - 1] = '#';
        game_state->board[i][DEFAULT_COL_NUM] = '\0';
    }
    for (int j = 1; j < DEFAULT_COL_NUM - 1; j++) {
        game_state->board[0][j] = '#';
        game_state->board[DEFAULT_ROW_NUM - 1][j] = '#';
    }
    game_state->board[2][2] = 'd';
    game_state->board[2][3] = '>';
    game_state->board[2][4] = 'D';
    game_state->board[2][9] = '*';

    // snake struct
    game_state->snakes->tail_row = 2;
    game_state->snakes->tail_col = 2;
    game_state->snakes->head_row = 2;
    game_state->snakes->head_col = 4;
    game_state->snakes->live = true;

    return game_state;
}

/* Task 2 */
void free_state(game_state_t* state) {
    for (int i = 0; i < state->num_rows; i++) {
        free(state->board[i]);
    }
    free(state->board);
    free(state->snakes);
    free(state);
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
    for (int i = 0; i < state->num_rows; i++) {
        fprintf(fp, "%s\n", state->board[i]);
    }
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
    FILE* f = fopen(filename, "w");
    print_board(state, f);
    fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int row, unsigned int col) {
    return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch) {
    state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
    const char* tail = "wasd";
    char* match = strchr(tail, c);
    return (match == NULL) ? false : true;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
    const char* head = "WASDx";
    char* match = strchr(head, c);
    return (match == NULL) ? false : true;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
    const char* snake = "wasd^<v>WASDx";
    char* match = strchr(snake, c);
    return (match == NULL) ? false : true;
}

/*
  Returns true if c is part of the wall.
*/
static bool is_wall(char c) {
    return c == '#';
}

/*
  Returns true if c is a fruit character.
*/
static bool is_fruit(char c) {
    return c == '*';
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
    if (c == '^') {
        return 'w';
    } else if (c == '<') {
        return 'a';
    } else if (c == 'v') {
        return 's';
    } else if (c == '>') {
        return 'd';
    } else {
        return c;
    }
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
    if (c == 'W') {
        return '^';
    } else if (c == 'A') {
        return '<';
    } else if (c == 'S') {
        return 'v';
    } else if (c == 'D') {
        return '>';
    } else {
        return c;
    }
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
    if (c == 'v' || c == 's' || c == 'S') {
        cur_row += 1;
    } else if (c == '^' || c == 'w' || c == 'W') {
        cur_row -= 1;
    }
    return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
    if (c == '>' || c == 'd' || c == 'D') {
        cur_col += 1;
    } else if (c == '<' || c == 'a' || c == 'A') {
        cur_col -= 1;
    }
    return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
    unsigned int head_row = state->snakes[snum].head_row;
    unsigned int head_col = state->snakes[snum].head_col;
    char head_char = get_board_at(state, head_row, head_col);

    unsigned int next_row = get_next_row(head_row, head_char);
    unsigned int next_col = get_next_col(head_col, head_char);
    return get_board_at(state, next_row, next_col);
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t* state, unsigned int snum) {
    unsigned int head_row = state->snakes[snum].head_row;
    unsigned int head_col = state->snakes[snum].head_col;
    char head_char = get_board_at(state, head_row, head_col);

    unsigned int next_row = get_next_row(head_row, head_char);
    unsigned int next_col = get_next_col(head_col, head_char);
    set_board_at(state, head_row, head_col, head_to_body(head_char));
    set_board_at(state, next_row, next_col, head_char);

    state->snakes[snum].head_row = next_row;
    state->snakes[snum].head_col = next_col;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
    unsigned int tail_row = state->snakes[snum].tail_row;
    unsigned int tail_col = state->snakes[snum].tail_col;
    char tail_char = get_board_at(state, tail_row, tail_col);

    unsigned int next_row = get_next_row(tail_row, tail_char);
    unsigned int next_col = get_next_col(tail_col, tail_char);
    char next_tail = get_board_at(state, next_row, next_col);

    set_board_at(state, next_row, next_col, body_to_tail(next_tail));
    set_board_at(state, tail_row, tail_col, ' ');

    state->snakes[snum].tail_row = next_row;
    state->snakes[snum].tail_col = next_col;
}

static void update_dead_snake(game_state_t* state, unsigned int snum) {
    unsigned int head_row = state->snakes[snum].head_row;
    unsigned int head_col = state->snakes[snum].head_col;
    set_board_at(state, head_row, head_col, 'x');
    state->snakes[snum].live = false;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
    for (unsigned int snum = 0; snum < state->num_snakes; snum++) {
        char next_char = next_square(state, snum);
        if (is_wall(next_char) || is_snake(next_char)) {
            update_dead_snake(state, snum);
        } else if (is_fruit(next_char)) {
            update_head(state, snum);
            add_food(state);
        } else {
            update_head(state, snum);
            update_tail(state, snum);
        }
    }
}

/* Task 5 */
game_state_t* load_board(FILE* fp) {
    game_state_t* state = (game_state_t *) malloc(sizeof(game_state_t));
    state->num_rows = 0;
    state->board = NULL;
    state->num_snakes = 0;
    state->snakes = NULL;

    // read from file
    char line[1000];
    while (fgets(line, sizeof(line), fp)) {
        size_t len = strlen(line);
        unsigned int idx = state->num_rows;
        state->num_rows += 1;
        state->board = (char **) realloc(state->board, sizeof(char *) * state->num_rows);
        state->board[idx] = (char *) malloc(len * sizeof(char));
        line[len - 1] = '\0';
        strcpy(state->board[idx], line);
    }
    return state;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
    unsigned int row = state->snakes[snum].tail_row;
    unsigned int col = state->snakes[snum].tail_col;
    char square = get_board_at(state, row, col);

    while (!is_head(square)) {
        row = get_next_row(row, square);
        col = get_next_col(col, square);
        square = get_board_at(state, row, col);
    }

    state->snakes[snum].head_row = row;
    state->snakes[snum].head_col = col;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
    for (unsigned int r = 0; r < state->num_rows; r++) {
        for (unsigned int c = 0; c < strlen(state->board[r]); c++) {
            char square = get_board_at(state, r, c);
            if (is_tail(square)) {
                unsigned int snum = state->num_snakes;
                state->num_snakes += 1;
                state->snakes = realloc(state->snakes, state->num_snakes * sizeof(snake_t));
                state->snakes[snum].tail_row = r;
                state->snakes[snum].tail_col = c;
                state->snakes[snum].live = true;
                find_head(state, snum);
            }
        }
    }
    return state;
}
