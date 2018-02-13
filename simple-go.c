#include "simple-go.h"

go_board* create_board(size_t size)
{
	go_board* board = malloc(sizeof(*board));
	assert(board);
	board->field_array = malloc(size*size*sizeof(*board->field_array));
	assert(board->field_array);
	for(size_t i = 0; i < size*size; i++)
	{
		board->field_array[i] = EMPTY;
	}
	board->size = size;

	return board;
}

void delete_board(go_board* board)
{
	free(board->field_array);
	free(board);
}

game_state* create_game(size_t size)
{
	game_state* game = malloc(sizeof(*game));
	game->board = create_board(size);
	game->black_turn = true;

	return game;
}

void delete_game(game_state* game)
{
	delete_board(game->board);
	free(game);
}

bool check_bounds(go_board* board, size_t y, size_t x)
{
	if(y < board->size && x < board->size)
		return true;
	else
		return false;
}

void kill_group(go_board* board, go_board* overlay)
{
	for(size_t y = 0; y < board->size; y++)
	{
		for(size_t x = 0; x < board->size; x++)
		{
			if(get_board_at(overlay, y, x) == GROUP)
				set_board_at(board, y, x, EMPTY);
		}
	}
}

void print_board(go_board* board)
{
	for(size_t y = 0; y < board->size; y++)
	{
		for(size_t x = 0; x < board->size; x++)
		{
			putchar(get_board_at(board,y,x));
			putchar(' ');
		}
		putchar('\n');
	}
}

bool play_at(game_state* game, size_t y, size_t x)
{
	//check out-of-bounds
	if(!check_bounds(game->board, y, x))
		return false;

	//check if field is empty
	if(get_board_at(game->board, y, x) != EMPTY)
		return false;

	bool can_place = false;

	char up = get_board_at(game->board, y-1, x);
	char left = get_board_at(game->board, y, x-1);
	char down = get_board_at(game->board, y+1, x);
	char right = get_board_at(game->board, y, x+1);

	//first check for group to kill, then for empty field, and last for group woth liberties
	if(up == (game->black_turn ? WHITE : BLACK))
	{
		go_board* enemy_group = create_board(game->board->size);
		find_group(game->board, enemy_group, y-1, x);
		if(count_liberties(game->board, enemy_group) <= 1)
		{
			kill_group(game->board, enemy_group);
			can_place = true;
		}
		delete_board(enemy_group);
	} else if(up == EMPTY) {
		can_place = true;
	} else {
		go_board* friendly_group = create_board(game->board->size);
		find_group(game->board, friendly_group, y-1, x);
		if(count_liberties(game->board, friendly_group) > 1)
			can_place = true;
		delete_board(friendly_group);
	}

	if(left == (game->black_turn ? WHITE : BLACK))
	{
		go_board* enemy_group = create_board(game->board->size);
		find_group(game->board, enemy_group, y, x-1);
		if(count_liberties(game->board, enemy_group) <= 1)
		{
			kill_group(game->board, enemy_group);
			can_place = true;
		}
	} else if(left == EMPTY) {
		can_place = true;
	} else {
		go_board* friendly_group = create_board(game->board->size);
		find_group(game->board, friendly_group, y, x-1);
		if(count_liberties(game->board, friendly_group) > 1)
			can_place = true;
		delete_board(friendly_group);
	}

	if(down == (game->black_turn ? WHITE : BLACK))
	{
		go_board* enemy_group = create_board(game->board->size);
		find_group(game->board, enemy_group, y+1, x);
		if(count_liberties(game->board, enemy_group) <= 1)
		{
			kill_group(game->board, enemy_group);
			can_place = true;
		}
	} else if(down == EMPTY) {
		can_place = true;
	} else {
		go_board* friendly_group = create_board(game->board->size);
		find_group(game->board, friendly_group, y+1, x);
		if(count_liberties(game->board, friendly_group) > 1)
			can_place = true;
		delete_board(friendly_group);
	}

	if(right == (game->black_turn ? WHITE : BLACK))
	{
		go_board* enemy_group = create_board(game->board->size);
		find_group(game->board, enemy_group, y, x+1);
		if(count_liberties(game->board, enemy_group) <= 1)
		{
			kill_group(game->board, enemy_group);
			can_place = true;
		}
	} else if(right == EMPTY) {
		can_place = true;
	} else {
		go_board* friendly_group = create_board(game->board->size);
		find_group(game->board, friendly_group, y, x+1);
		if(count_liberties(game->board, friendly_group) > 1)
			can_place = true;
		delete_board(friendly_group);
	}

	if(!can_place)
		return false;

	set_board_at(game->board, y, x, game->black_turn ? BLACK : WHITE);
	game->black_turn = !game->black_turn;
	return true;
}

char get_board_at(go_board* board, size_t y, size_t x)
{
	if(check_bounds(board, y, x))
		return board->field_array[y*board->size+x];
	else
		return INVALID_FIELD;
}

void set_board_at(go_board* board, size_t y, size_t x, char item)
{
	if(check_bounds(board,y,x))
		board->field_array[y*board->size+x] = item;
}

void find_group(go_board* board, go_board* overlay, size_t y, size_t x)
{
	assert(board->size == overlay->size);
	assert(check_bounds(board, y, x));

	set_board_at(overlay, y, x, GROUP);
	char field = get_board_at(board,y,x);

	if(get_board_at(board,y-1,x) == field && get_board_at(overlay,y-1,x) == EMPTY)
		find_group(board, overlay, y-1, x);

	if(get_board_at(board,y,x-1) == field && get_board_at(overlay,y,x-1) == EMPTY)
		find_group(board, overlay, y, x-1);

	if(get_board_at(board,y+1,x) == field && get_board_at(overlay,y+1,x) == EMPTY)
		find_group(board, overlay, y+1, x);

	if(get_board_at(board,y,x+1) == field && get_board_at(overlay,y,x+1) == EMPTY)
		find_group(board, overlay, y, x+1);
}

size_t count_liberties(go_board* board, go_board* overlay)
{
	assert(board->size == overlay->size);

	go_board* tmpoverlay = create_board(board->size);
	memcpy(tmpoverlay->field_array, overlay->field_array, board->size*board->size);

	size_t liberties = 0;

	for(size_t y = 0; y < board->size; y++)
	{
		for(size_t x = 0; x < board->size; x++)
		{
			if(get_board_at(overlay, y, x) == GROUP)
			{
				if(get_board_at(board,y-1,x) == EMPTY && get_board_at(tmpoverlay,y-1,x) != COUNTED)
				{
					liberties++;
					set_board_at(tmpoverlay,y-1,x,COUNTED);
				}
				if(get_board_at(board,y,x-1) == EMPTY && get_board_at(tmpoverlay,y,x-1) != COUNTED)
				{
					liberties++;
					set_board_at(tmpoverlay,y,x-1,COUNTED);
				}
				if(get_board_at(board,y+1,x) == EMPTY && get_board_at(tmpoverlay,y+1,x) != COUNTED)
				{
					liberties++;
					set_board_at(tmpoverlay,y+1,x,COUNTED);
				}
				if(get_board_at(board,y,x+1) == EMPTY && get_board_at(tmpoverlay,y,x+1) != COUNTED)
				{
					liberties++;
					set_board_at(tmpoverlay,y,x+1,COUNTED);
				}

			}
		}
	}

	delete_board(tmpoverlay);

	return liberties;
}


