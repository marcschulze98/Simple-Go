#ifndef SIMPLE_GO_H
#define SIMPLE_GO_H
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <cutils/cutils.h>

#define BLACK 'b'
#define WHITE 'w'
#define EMPTY '.'
#define GROUP '#'
#define NO_FIELD '\0'
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
	unsigned long white_captured;
	unsigned long black_captured;
	bool black_turn;
} game_state;

typedef struct go_score
{
	Vector* white_groups;
	Vector* black_groups;
	unsigned long white_points;
	unsigned long black_points;
} go_score;


go_board* create_board(go_coordinate size);
void delete_board(go_board* board);
game_state* create_game(go_coordinate size, float komi);
void delete_game(game_state* game);

bool check_bounds(const go_board* board, go_coordinate y, go_coordinate x);
char get_board_at(const go_board* board, go_coordinate y, go_coordinate x);
void set_board_at(go_board* board, go_coordinate y, go_coordinate x, char item);

void kill_group(go_board* board, const go_board* overlay);
bool group_killable(game_state* game, go_coordinate y, go_coordinate x);
bool group_attachable(const game_state* game, go_coordinate y, go_coordinate x);
bool play_at(game_state* game, go_coordinate y, go_coordinate x, go_symbol color);

void print_board(const go_board* board);
char* board_to_string(const go_board* board);
void find_group(const go_board* board, go_board* overlay, go_coordinate y, go_coordinate x);
go_symbol group_belongs(const go_board* board, const go_board* overlay);
unsigned long count_liberties(const go_board* board, const go_board* overlay);
unsigned long group_size(go_board* group);

go_score* score_game(const game_state* board);
void delete_score(go_score* score);






#endif //SIMPLE_GO_H
