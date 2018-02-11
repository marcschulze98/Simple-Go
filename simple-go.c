#include "simple-go.h"

go_board* create_board(size_t size)
{
	go_board* board = malloc(sizeof(*board));
	board->field_array = malloc(size*size*sizeof(*board->field_array));
	for(size_t i = 0; i < size; i++)
	{
		board->field_array[i] = EMPTY;
	}
	board->size = size;

	return board;
}

void print_board(go_board* board)
{
	for(size_t y = 0; y < board->size; y++)
	{
		for(size_t x = 0; x < board->size; x++)
		{
			putchar(get_board_at(board,y,x));
		}
		putchar('\n');
	}
}

char get_board_at(go_board* board, size_t y, size_t x)
{
	if(y >= 0  && x >= 0 && y < board->size && x < board->size)
		return board->field_array[y*board->size+x];
	else
		return INVALID_FIELD;
}

void set_board_at(go_board* board, size_t y, size_t x, char item)
{
	if(y >= 0  && x >= 0 && y < board->size && x < board->size)
		board->field_array[y*board->size+x] = item;
}

void find_group(go_board* board, go_board* overlay, size_t y, size_t x)
{
	set_board_at(overlay, y-1, x, GROUP);
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


