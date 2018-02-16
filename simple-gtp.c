#include "simple-gtp.h"

char* cmd_error(const char* msg)
{
	char* ret = malloc(strlen("? ") + strlen(msg) + 1);
	strcpy(ret, "? ");
	strcat(ret, msg);
	return ret;
}

char* cmd_success(const char* msg)
{
	char* ret = malloc(strlen("= ") + strlen(msg) + 2);
	strcpy(ret, "= ");
	strcat(ret, msg);
	return ret;
}

const char* const known_commands_string = "protocol_version\nname\nversion\nknown_command\nlist_commands\nquit\nboardsize\nclear_board\nkomi\nplay\ngenmove\nshowboard";
const char* known_commands_array[] = {"protocol_version","name","version","known_command","list_commands","quit","boardsize","clear_board","komi","play","genmove","showboard"};
int cmd_count = sizeof(known_commands_array)/sizeof(known_commands_array[0]);

char* handle_gtp_cmd(const char* msg, game_state* game)
{
	if(!msg || !strlen(msg))
	{
		char* ret = calloc(1,1);
		return ret;
	}

	char* command = malloc(strlen(msg) + 1);
	sscanf(msg, "%s", command);

	Vector* arguments = new_vector();

	char* tmp_msg = malloc(strlen(msg)+1);
	strcpy(tmp_msg, msg);


	char* current = strtok(tmp_msg, " ");
	char* tmp_arg;

	while(current)
	{
		tmp_arg = malloc(strlen(current)+1);
		strcpy(tmp_arg, current);
		vector_push(arguments, tmp_arg);
		current = strtok(NULL, " ");
	}

	char* (*func_ptr)(const char*) = cmd_error;
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
		unsigned int size = game->board->size;
		delete_board(game->board);
		game->board = create_board(size);
	} else if(strcmp(command, "play") == 0) {
		char* color = malloc(6);
		char y;
		int x;
		if((arguments->length == 2 || arguments->length == 3) &&
		   (snprintf(color, 6, "%s", (char*)vector_at(arguments,0)) > 0) &&
		   (strcmp(color, "white") == 0 || strcmp(color, "black") == 0) &&
		   (sscanf(vector_at(arguments,1), "%c", &y) > 0) &&
		   ((unsigned char)(tolower(y) - 'a') < game->board->size) &&
		   (sscanf((char*)vector_at(arguments,1)+1, "%d", &x) > 0) &&
		   ((unsigned int)x <= game->board->size) && (unsigned int)x > 0)
		{
			if(play_at(game, (unsigned char)(y-'a'), (unsigned int)x-1))
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
		unsigned int y;
		unsigned int x;
		do
		{
			y = (unsigned int)rand() % game->board->size;
			x = (unsigned int)rand() % game->board->size;
		} while(!play_at(game, y, x));

	}

	free(command);
	delete_vector(arguments, NULL);
	free(arguments);
	free(tmp_msg);

	return func_ptr(func_args);
}
