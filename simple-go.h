#ifndef SIMPLE_GO_H
#define SIMPLE_GO_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#define BLACK 'b'
#define WHITE 'w'
#define EMPTY '.'
#define GROUP '#'
#define INVALID_FIELD '\0'
#define COUNTED '+'

typedef struct go_board
{
	char* field_array;
	size_t size;
} go_board;

typedef struct game_state
{
	go_board* board;
	bool black_turn;
} game_state;


go_board* create_board(size_t size);
bool play_at(game_state* game, size_t y, size_t x);
bool check_bounds(go_board* board, size_t y, size_t x);
void delete_board(go_board* board);
char get_board_at(go_board* board, size_t y, size_t x);
void set_board_at(go_board* board, size_t y, size_t x, char item);
void print_board(go_board* board);
void find_group(go_board* board, go_board* overlay, size_t y, size_t x);
size_t count_liberties(go_board* board, go_board* overlay);





#endif //SIMPLE_GO_H
