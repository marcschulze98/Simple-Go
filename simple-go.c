#include "simple-go.h"

go_board* create_board(go_coordinate size)
{
	go_board* board = malloc(sizeof(*board));
	assert(board);
	board->field_array = malloc(size*size*sizeof(*board->field_array));
	assert(board->field_array);
	for(go_coordinate i = 0; i < size*size; i++)
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

game_state* create_game(go_coordinate size, float komi)
{
	game_state* game = malloc(sizeof(*game));
	game->board = create_board(size);
	game->black_turn = true;
	game->komi = komi;

	return game;
}

void delete_game(game_state* game)
{
	delete_board(game->board);
	free(game);
}

bool check_bounds(const go_board* board, go_coordinate y, go_coordinate x)
{
	if(y < board->size && x < board->size)
		return true;
	else
		return false;
}

void kill_group(go_board* board, go_board* overlay)
{
	for(go_coordinate y = 0; y < board->size; y++)
	{
		for(go_coordinate x = 0; x < board->size; x++)
		{
			if(get_board_at(overlay, y, x) == GROUP)
				set_board_at(board, y, x, EMPTY);
		}
	}
}

void print_board(go_board* board)
{
	for(go_coordinate y = 0; y < board->size; y++)
	{
		for(go_coordinate x = 0; x < board->size; x++)
		{
			putchar(get_board_at(board,y,x));
			putchar(' ');
		}
		putchar('\n');
	}
}



bool group_attachable(const game_state* game, go_coordinate y, go_coordinate x)
{
	go_board* friendly_group = create_board(game->board->size);
	find_group(game->board, friendly_group, y, x);
	if(count_liberties(game->board, friendly_group) > 1)
	{
		delete_board(friendly_group);
		return true;
	} else {
		delete_board(friendly_group);
		return false;
	}
}

bool group_killable(game_state* game, go_coordinate y, go_coordinate x)
{
	go_board* enemy_group = create_board(game->board->size);
	find_group(game->board, enemy_group, y, x);
	if(count_liberties(game->board, enemy_group) <= 1)
	{
		kill_group(game->board, enemy_group);
		delete_board(enemy_group);
		return true;
	} else {
		delete_board(enemy_group);
		return false;
	}
}

bool play_at(game_state* game, go_coordinate y, go_coordinate x)
{
	//check out-of-bounds
	if(!check_bounds(game->board, y, x))
		return false;

	//check if field is empty
	if(get_board_at(game->board, y, x) != EMPTY)
		return false;

	bool can_place = false;

	go_symbol enemy = game->black_turn ? WHITE : BLACK;
	go_symbol friendly = game->black_turn ? BLACK : WHITE;

	go_symbol up = y == 0 ? INVALID_FIELD : get_board_at(game->board, y-1, x);
	go_symbol left = x == 0 ? INVALID_FIELD : get_board_at(game->board, y, x-1);
	go_symbol down = y == game->board->size-1 ? INVALID_FIELD : get_board_at(game->board, y+1, x);
	go_symbol right = x == game->board->size-1 ? INVALID_FIELD : get_board_at(game->board, y, x+1);

	//first check for group to kill, then for empty field, and last for group with liberties

	if(up == enemy && group_killable(game, y-1, x))
		can_place = true;
	else if(up == EMPTY)
		can_place = true;
	else if(up == friendly && group_attachable(game, y-1, x))
		can_place = true;

	if(left == enemy && group_killable(game, y, x-1))
		can_place = true;
	else if(left == EMPTY)
		can_place = true;
	else if(left == friendly && group_attachable(game, y, x-1))
		can_place = true;

	if(down == enemy && group_killable(game, y+1, x))
		can_place = true;
	else if(down == EMPTY)
		can_place = true;
	else if(down == friendly && group_attachable(game, y+1, x))
		can_place = true;

	if(right == enemy && group_killable(game, y, x+1))
		can_place = true;
	else if(right == EMPTY)
		can_place = true;
	else if(right == friendly && group_attachable(game, y, x+1))
		can_place = true;

	if(!can_place)
		return false;

	set_board_at(game->board, y, x, game->black_turn ? BLACK : WHITE);
	game->black_turn = !game->black_turn;
	return true;
}

go_symbol get_board_at(const go_board* board, go_coordinate y, go_coordinate x)
{
	if(check_bounds(board, y, x))
		return board->field_array[y*board->size+x];
	else
		return INVALID_FIELD;
}

void set_board_at(go_board* board, go_coordinate y, go_coordinate x, go_symbol item)
{
	if(check_bounds(board,y,x))
		board->field_array[y*board->size+x] = item;
}

void find_group(const go_board* board, go_board* overlay, go_coordinate y, go_coordinate x)
{
	assert(board->size == overlay->size);
	if(check_bounds(board, y, x))
	{
		set_board_at(overlay, y, x, GROUP);
		go_symbol field = get_board_at(board,y,x);

		if(get_board_at(board,y-1,x) == field && get_board_at(overlay,y-1,x) == EMPTY)
			find_group(board, overlay, y-1, x);

		if(get_board_at(board,y,x-1) == field && get_board_at(overlay,y,x-1) == EMPTY)
			find_group(board, overlay, y, x-1);

		if(get_board_at(board,y+1,x) == field && get_board_at(overlay,y+1,x) == EMPTY)
			find_group(board, overlay, y+1, x);

		if(get_board_at(board,y,x+1) == field && get_board_at(overlay,y,x+1) == EMPTY)
			find_group(board, overlay, y, x+1);
	}
}

unsigned long count_liberties(go_board* board, go_board* overlay)
{
	assert(board->size == overlay->size);

	go_board* tmpoverlay = create_board(board->size);
	memcpy(tmpoverlay->field_array, overlay->field_array, board->size*board->size);

	unsigned long liberties = 0;

	for(go_coordinate y = 0; y < board->size; y++)
	{
		for(go_coordinate x = 0; x < board->size; x++)
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


