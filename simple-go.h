#ifndef SIMPLE_GO_H
#define SIMPLE_GO_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define BLACK 'b'
#define WHITE 'w'
#define EMPTY ' '
#define GROUP '#'
#define INVALID_FIELD '\0'

typedef struct go_board
{
	char* field_array;
	size_t size;
} go_board;

typedef struct game_state
{
	struct go_board* board;
	bool black_turn;
} game_state;


go_board* create_board(size_t size);
char get_board_at(go_board* board, size_t y, size_t x);
void set_board_at(go_board* board, size_t y, size_t x, char item);


#endif //SIMPLE_GO_H
