#include "simple-go.h"

go_board* create_board(size_t size)
{
	go_board* board = malloc(sizeof(*board));
	board->field_array = malloc(size*size*sizeof(*board->field_array));
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
#include <unistd.h>
void find_group(go_board* board, go_board* overlay, size_t y, size_t x)
{
	assert(board->size == overlay->size);

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

	return liberties;
}


