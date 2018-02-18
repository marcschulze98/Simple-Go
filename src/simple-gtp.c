#include <simple-go/simple-gtp.h>

typedef struct msg_formatted
{
	char* command;
	Vector* arguments;
	char* id;
} msg_formatted;

char* cmd_error(const char* msg, char* id)
{
	char* ret = malloc(strlen("? ") + strlen(msg) + (id ? strlen(id) : 0) + 3);
	strcpy(ret, "?");
	if(id)
	{
		strcat(ret, id);
		free(id);
	}
	strcat(ret, " ");
	strcat(ret, msg);
	strcat(ret, "\n\n");
	return ret;
}

char* cmd_success(const char* msg, char* id)
{
	char* ret = malloc(strlen("= ") + strlen(msg) + (id ? strlen(id) : 0) + 3);
	strcpy(ret, "=");
	if(id)
	{
		strcat(ret, id);
		free(id);
	}
	strcat(ret, " ");
	strcat(ret, msg);
	strcat(ret, "\n\n");
	return ret;
}

const char* const known_commands_string = "protocol_version\nname\nversion\nknown_command\nlist_commands\nquit\nboardsize\nclear_board\nkomi\nplay\ngenmove\nshowboard";
const char* known_commands_array[] = {"protocol_version","name","version","known_command","list_commands","quit","boardsize","clear_board","komi","play","genmove","showboard"};
int cmd_count = sizeof(known_commands_array)/sizeof(known_commands_array[0]);

static msg_formatted format_msg(const char* msg)
{
	msg_formatted formatted = {0};

	size_t index = 0;

	while(isdigit(msg[index]) || msg[index] == '-')
		index++;

	if(index)
	{
		formatted.id = malloc(index+1);
		strncpy(formatted.id, msg, index);
		formatted.id[index] = '\0';
	}

	formatted.command = malloc(strlen(msg+index) + 1);
	sscanf(msg+index, "%s", formatted.command);

	formatted.arguments = new_vector();

	char* tmp_msg = malloc(strlen(msg+index)+1);
	strcpy(tmp_msg, msg+index);


	char* current = strtok(tmp_msg, " ");
	char* tmp_arg;

	while(current)
	{
		tmp_arg = malloc(strlen(current)+1);
		strcpy(tmp_arg, current);
		vector_push(formatted.arguments, tmp_arg);
		current = strtok(NULL, " ");
	}

	if(formatted.arguments->length > 1)
		vector_remove(formatted.arguments, 0, free);

	free(tmp_msg);

	return formatted;
}

char* handle_gtp_cmd(const char* msg, game_state* game)
{
	if(!msg || !strlen(msg))
	{
		char* ret = calloc(1,1);
		return ret;
	}

	msg_formatted formatted = format_msg(msg);
	char* command = formatted.command;
	Vector* arguments = formatted.arguments;
	char* id = formatted.id;

	char* (*func_ptr)(const char*, char* id) = cmd_error;
	const char* func_args = "unknown command";

	if(strcmp(command, "protocol_version") == 0)
	{
		func_ptr = cmd_success;
		func_args = "2";
	} else if(strcmp(command, "name") == 0) {
		func_ptr = cmd_success;
		func_args = "simple-go";
	} else if(strcmp(command, "version") == 0) {
		func_ptr = cmd_success;
		func_args = "0.1";
	} else if(strcmp(command, "known_command") == 0) {
		func_ptr = cmd_success;
		func_args = "false";
		for(int i = 0; i < cmd_count; i++)
		{
			if(arguments->length > 0 && strcmp(vector_at(arguments, 0), known_commands_array[i]) == 0)
			{
				func_args = "true";
				break;
			}
		}
	} else if(strcmp(command, "list_commands") == 0) {
		func_ptr = cmd_success;
		func_args = known_commands_string;
	} else if(strcmp(command, "boardsize") == 0) {
		unsigned int size;
		if(arguments->length == 1 && sscanf(vector_at(arguments,0), "%u", &size) > 0 && 0 < size && size < 26)
		{
			func_ptr = cmd_success;
			func_args = "";
			delete_board(game->board);
			game->board = create_board(size);
		} else {
			func_ptr = cmd_error;
			func_args = "unacceptable size";
		}
	} else if(strcmp(command, "komi") == 0) {
		float komi;
		if(arguments->length == 1 && sscanf(vector_at(arguments,0), "%f", &komi))
		{
			func_ptr = cmd_success;
			func_args = "";
			game->komi = komi;
		} else {
			func_ptr = cmd_error;
			func_args = "komi not a float";
		}
	} else if(strcmp(command, "clear_board") == 0) {
		func_ptr = cmd_success;
		func_args = "";
		unsigned int size = game->board->size;
		delete_board(game->board);
		game->board = create_board(size);
	} else if(strcmp(command, "play") == 0) {
		char* color = malloc(6);
		char x;
		int y;
		if((arguments->length == 2 || arguments->length == 3) && //either its "a 10" or "a10"
		   (snprintf(color, 6, "%s", (char*)vector_at(arguments,0)) > 0) && //"white" and "black" are only 6 chars
		   (strcmp(color, "white") == 0 || strcmp(color, "black") == 0) && // check color
		   (sscanf(vector_at(arguments,1), "%c", &x) > 0) && // get horizontal coordinate
		   ((unsigned char)(tolower(x) - 'a') < game->board->size) && // check range
		   ((sscanf((char*)vector_at(arguments,1)+1, "%d", &y) > 0) || // get vertical coordinate if no space between
		   ((arguments->length == 3) && (sscanf((char*)vector_at(arguments,2), "%d", &y) > 0))) && // get vertical coordinate if space between
		   ((unsigned int)y <= game->board->size) && y > 0) // check range
		{
			if(play_at(game, game->board->size-(unsigned int)y, (unsigned char)(x-'a'), strcmp(color, "white") == 0 ? WHITE : BLACK))
			{
				func_ptr = cmd_success;
				func_args = "";
			} else {
				func_ptr = cmd_error;
				func_args = "illegal move";
			}
		} else {
			func_ptr = cmd_error;
			func_args = "invalid color or coordinate";
		}
		free(color);
	} else if(strcmp(command, "genmove") == 0) {
		func_ptr = cmd_success;
		func_args = "";
		go_coordinate y;
		go_coordinate x;
		do
		{
			y = (go_coordinate)rand() % game->board->size;
			x = (go_coordinate)rand() % game->board->size;
		} while(!play_at(game, y, x, NO_FIELD));
	} else if(strcmp(command, "showboard") == 0) {
		char* board = board_to_string(game->board);

		free(command);
		delete_vector(arguments, free);
		char* tmp = cmd_success(board, id);
		free(board);
		return tmp;
	}

	free(command);
	delete_vector(arguments, free);

	return func_ptr(func_args, id);
}
