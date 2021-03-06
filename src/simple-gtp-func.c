#include <simple-go/simple-gtp-func.h>

const char* const known_commands_string = "protocol_version\nname\nversion\nknown_command\nlist_commands\nquit\nboardsize\nclear_board\nkomi\nplay\ngenmove\nshowboard\nfinal_score";
const char* known_commands_array[] = {"protocol_version","name","version","known_command","list_commands","quit","boardsize","clear_board","komi","play","genmove","showboard", "final_score", NULL};

static char* cmd_error(const char* msg, const char* id)
{
	char* ret = malloc(strlen("? ") + (msg ? strlen(msg) : 0) + (id ? strlen(id) : 0) + 3);
	strcpy(ret, "?");

	if(id)
		strcat(ret, id);

	if(msg)
	{
		strcat(ret, " ");
		strcat(ret, msg);
	}
	strcat(ret, "\n\n");
	return ret;
}

static char* cmd_success(const char* msg, const char* id)
{
	char* ret = malloc(strlen("= ") + (msg ? strlen(msg) : 0) + (id ? strlen(id) : 0) + 3);
	strcpy(ret, "=");

	if(id)
		strcat(ret, id);

	if(msg)
	{
		strcat(ret, " ");
		strcat(ret, msg);
	}
	strcat(ret, "\n\n");
	return ret;
}

char* unkown_command_func(const Vector* arguments, const char* id, game_state* game)
{
	return cmd_error("unknown command", id);
}

char* protocol_version_func(const Vector* arguments, const char* id, game_state* game)
{
	return cmd_success("2", id);
}

char* name_func(const Vector* arguments, const char* id, game_state* game)
{
	return cmd_success("simple-go", id);
}

char* version_func(const Vector* arguments, const char* id, game_state* game)
{
	return cmd_success(VERSIONSTR, id);
}

char* known_command_func(const Vector* arguments, const char* id, game_state* game)
{
	for(size_t i = 0;  known_commands_array[i]; i++)
	{
		if(arguments->length > 0 && strcmp(vector_at(arguments, 0), known_commands_array[i]) == 0)
		{
			return cmd_success("false", id);
		}
	}
	return cmd_success("false", id);
}

char* list_commands_func(const Vector* arguments, const char* id, game_state* game)
{
	return cmd_success(known_commands_string, id);
}

char* boardsize_func(const Vector* arguments, const char* id, game_state* game)
{
	unsigned int size;
	if(arguments->length == 1 && sscanf(vector_at(arguments,0), "%u", &size) > 0 && 0 < size && size < 26)
	{
		delete_board(game->board);
		game->board = create_board(size);
		return cmd_success(NULL, id);
	} else {
		return cmd_error("unacceptable size", id);
	}
}

char* komi_func(const Vector* arguments, const char* id, game_state* game)
{
	float komi;
	if(arguments->length == 1 && sscanf(vector_at(arguments,0), "%f", &komi))
	{
		game->komi = komi;
		return cmd_success(NULL, id);
	} else {
		return cmd_error("komi not a float", id);
	}
}

char* clear_board_func(const Vector* arguments, const char* id, game_state* game)
{
	go_coordinate size = game->board->size;
	delete_board(game->board);
	game->board = create_board(size);
	return cmd_success(NULL, id);
}

char* play_func(const Vector* arguments, const char* id, game_state* game)
{
	char* color = malloc(6);
	char xchar;
	size_t x;
	size_t y;

	if(!(arguments->length == 2 || arguments->length == 3)) //allow "a 10" or "a10"
		goto error;
	if(snprintf(color, 6, "%s", (char*)vector_at(arguments,0)) <= 0) //"white" and "black" are only 6 chars
		goto error;
	if(!(strcmp_nocase(color, "white") == 0 || strcmp_nocase(color, "black") == 0 || // check color
	   strcmp_nocase(color, "w") == 0 || strcmp_nocase(color, "b") == 0))
		goto error;
	if(sscanf(vector_at(arguments,1), "%c", &xchar) <= 0) // get horizontal coordinate
		goto error;

	x = (size_t)tolower(xchar) - 'a';
	x = x >= 9 ? x-1 : x; //skip J
	if((x < 'i' && x >= game->board->size) || (x > 'i' && x-1 >= game->board->size)) // check range
		goto error;
	if(!((arguments->length == 2) && (sscanf((char*)vector_at(arguments,1)+1, "%zu", &y) > 0)) && // get vertical coordinate if no space between
	   !((arguments->length == 3) && (sscanf((char*)vector_at(arguments,2), "%zu", &y) > 0)))     // get vertical coordinate if space between
		goto error;
	if(y > game->board->size)
		goto error;

	if(play_at(game, game->board->size-y, x, (strcmp(color, "white") == 0 || strcmp(color, "w") == 0) ? WHITE : BLACK))
	{
		free(color);
		return cmd_success(NULL, id);
	} else {
		free(color);
		return cmd_error("illegal move", id);
	}

error:
	free(color);
	return cmd_error("invalid color or coordinate", id);
}

char* genmove_func(const Vector* arguments, const char* id, game_state* game)
{
	char* color = malloc(6);
	if(arguments->length == 1 &&
	(snprintf(color, 6, "%s", (char*)vector_at(arguments,0)) > 0) &&
	(strcmp(color, "white") == 0 || strcmp(color, "black") == 0 ||
	strcmp(color, "w") == 0 || strcmp(color, "b") == 0 ))
	{
		go_coordinate y;
		go_coordinate x;
		do
		{
			y = (go_coordinate)rand() % game->board->size;
			x = (go_coordinate)rand() % game->board->size;
		} while(!play_at(game, y, x, (strcmp(color, "white") == 0 || strcmp(color, "w") == 0) ? WHITE : BLACK));
		free(color);
		return cmd_success(NULL, id);
	} else {
		free(color);
		return cmd_error("invalid color or coordinate", id);
	}
}

char* show_board_func(const Vector* arguments, const char* id, game_state* game)
{
	char* board = board_to_string(game->board);
	char* tmp = cmd_success(board, id);
	free(board);
	return tmp;
}

char* final_score_func(const Vector* arguments, const char* id, game_state* game)
{
	go_score* score = score_game(game);
	char* result = calloc(20,1);

	if(score->white_points > score->black_points)
		sprintf(result,"%c%c%.1lf", 'W', '+', score->white_points-score->black_points);
	else if(score->black_points > score->white_points)
		sprintf(result,"%c%c%.1lf", 'B', '+', score->black_points-score->white_points);
	else
		sprintf(result,"0");

	char* tmp = cmd_success(result, id);

	free(result);
	delete_score(score);

	return tmp;
}
