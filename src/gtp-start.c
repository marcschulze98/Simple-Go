#include <simple-go/simple-go.h>
#include <simple-go/simple-gtp.h>

int main(int argc, char** argv)
{
	game_state* game = create_game(19, 6.5);
	char input[50];
	char* answer;

	while(1)
	{
		fgets(input, 50, stdin);
		if(strcmp(input, "quit\n") == 0)
			break;
		answer = handle_gtp_cmd(input, game);
		printf("%s", answer);
		free(answer);
	}

	puts("Bye!");
	delete_game(game);
}
