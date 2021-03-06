#include <simple-go/simple-go.h>
#include <simple-go/simple-gtp.h>

static void test1(void)
{
	puts("test1 running");
	game_state* test_game = create_game(5, 6.5);
	go_board* test_board = test_game->board;

	set_board_at(test_board, 2, 2, WHITE);
	set_board_at(test_board, 2, 3, WHITE);
	set_board_at(test_board, 1, 3, WHITE);
	set_board_at(test_board, 3, 3, WHITE);
	set_board_at(test_board, 4, 4, WHITE);
	set_board_at(test_board, 3, 1, WHITE);
	set_board_at(test_board, 3, 2, BLACK);

	print_board(test_board);
	putchar('\n');

	go_board* test_overlay = create_board(5);
	find_group(test_board, test_overlay, 2, 2);

	print_board(test_overlay);

	printf("liberties: %ld\n", count_liberties(test_board,test_overlay));

	kill_group(test_game, test_overlay);

	print_board(test_board);

	delete_game(test_game);
	delete_board(test_overlay);
	puts("test1 finished");
}

static void test2(void)
{
	puts("test2 running");
	game_state* test_game = create_game(5, 0);
	go_board* test_board = test_game->board;
	set_board_at(test_board, 2, 3, WHITE);
	set_board_at(test_board, 3, 2, WHITE);
	set_board_at(test_board, 1, 2, WHITE);
	set_board_at(test_board, 2, 1, WHITE);
	set_board_at(test_board, 1, 1, BLACK);

	play_at(test_game, 2, 2, NO_FIELD);
	print_board(test_game->board);

	set_board_at(test_board, 2, 1, EMPTY);

	play_at(test_game, 2, 2, NO_FIELD);
	print_board(test_game->board);


	play_at(test_game, 2, 1, NO_FIELD);
	print_board(test_game->board);

	delete_game(test_game);
	puts("test2 finished");
}

static void test3(void)
{
	puts("test3 running");

	game_state* test_game = create_game(11, 0);

	char* ret =handle_gtp_cmd("play white a 11", test_game);
	printf("%s\n", ret);
	free(ret);
	print_board(test_game->board);

	ret =handle_gtp_cmd("showboard", test_game);
	printf("%s\n", ret);
	free(ret);

	ret =handle_gtp_cmd("version", test_game);
	printf("%s\n", ret);
	free(ret);

	ret =handle_gtp_cmd("komi -6.5", test_game);
	printf("%s\n", ret);
	printf("%f\n", test_game->komi);
	free(ret);

	ret =handle_gtp_cmd("protocol_version", test_game);
	printf("%s\n", ret);
	free(ret);

	ret =handle_gtp_cmd("known_command play", test_game);
	printf("%s\n", ret);
	free(ret);

	ret =handle_gtp_cmd("known_command what", test_game);
	printf("%s\n", ret);
	free(ret);

	ret =handle_gtp_cmd("list_commands", test_game);
	printf("%s\n", ret);
	free(ret);

	ret =handle_gtp_cmd("1clear_board", test_game);
	printf("%s\n", ret);
	free(ret);

	ret =handle_gtp_cmd("2boardsize 26", test_game);
	printf("%s\n", ret);
	free(ret);

	ret =handle_gtp_cmd("genmove", test_game);
	printf("%s\n", ret);
	free(ret);

	ret =handle_gtp_cmd("final_score", test_game);
	printf("%s\n", ret);
	free(ret);

	print_board(test_game->board);


	delete_game(test_game);

	puts("test3 finished");
}

static void test4(void)
{
	puts("test4 running");

	game_state* test_game = create_game(19, 6.5);
	go_board* test_board = test_game->board;
	set_board_at(test_board, 2, 3, WHITE);
	set_board_at(test_board, 2, 5, WHITE);
	set_board_at(test_board, 1, 4, WHITE);
	set_board_at(test_board, 3, 4, BLACK);
	play_at(test_game, 2,4, NO_FIELD);

	print_board(test_game->board);

	delete_game(test_game);
	puts("test4 finished");
}

static void test5(void)
{
	puts("test5 running");
	game_state* game = create_game(19,0);
	set_board_at(game->board, 2, 3, WHITE);
	set_board_at(game->board, 2, 5, WHITE);
	set_board_at(game->board, 1, 4, WHITE);
	set_board_at(game->board, 3, 4, WHITE);

	set_board_at(game->board, 5, 6, WHITE);
	set_board_at(game->board, 5, 8, WHITE);
	set_board_at(game->board, 4, 7, WHITE);
	set_board_at(game->board, 6, 7, BLACK);

	set_board_at(game->board, 5, 2, BLACK);
	set_board_at(game->board, 5, 4, BLACK);
	set_board_at(game->board, 4, 3, BLACK);
	set_board_at(game->board, 6, 3, BLACK);

	go_score* score = score_game(game);
	print_board(game->board);
	printf("White groups: %ld\n", score->white_groups->length);
	printf("White points: %lf\n", score->white_points);
	printf("Black groups: %ld\n", score->black_groups->length);
	printf("Black points: %lf\n", score->black_points);


	delete_score(score);
	delete_game(game);
	puts("test5 finished");
}


int main(int argc, char** argv)
{

	test1();
	test2();
	test3();
	test4();
	test5();

}
