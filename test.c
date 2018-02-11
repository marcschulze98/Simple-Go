#include "simple-go.h"

int main(int argc, char** argv)
{
	go_board* test_board = create_board(5);

	set_board_at(test_board,2,2,WHITE);
	set_board_at(test_board,2,3,WHITE);
	set_board_at(test_board,1,3,WHITE);
	set_board_at(test_board,3,3,WHITE);

	print_board(test_board);
	putchar('\n');

	go_board* overlay = create_board(5);
	find_group(test_board, overlay,2,2);

	print_board(overlay);

	printf("liberties: %ld\n", count_liberties(test_board,overlay));

}
