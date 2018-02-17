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

typedef unsigned int go_coordinate;
typedef char go_symbol;

typedef struct go_board
{
	go_symbol* field_array;
	go_coordinate size;
} go_board;

typedef struct game_state
{
	go_board* board;
	float komi;
	bool black_turn;
} game_state;


go_board* create_board(unsigned int size);
void delete_board(go_board* board);
game_state* create_game(unsigned int size, float komi);
void delete_game(game_state* game);

bool check_bounds(const go_board* board, unsigned int y, unsigned int x);
char get_board_at(const go_board* board, unsigned int y, unsigned int x);
void set_board_at(go_board* board, unsigned int y, unsigned int x, char item);

void kill_group(go_board* board, go_board* overlay);
bool group_killable(game_state* game, unsigned int y, unsigned int x);
bool group_attachable(const game_state* game, unsigned int y, unsigned int x);
bool play_at(game_state* game, unsigned int y, unsigned int x);

void print_board(go_board* board);
void find_group(const go_board* board, go_board* overlay, unsigned int y, unsigned int x);
unsigned long count_liberties(go_board* board, go_board* overlay);





#endif //SIMPLE_GO_H
