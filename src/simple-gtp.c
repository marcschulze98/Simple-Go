#include <simple-go/simple-gtp.h>
#include <simple-go/simple-gtp-func.h>

typedef struct msg_formatted
{
	char* command;
	Vector* arguments;
	char* id;
} msg_formatted;


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

static char* sanitize(const char* msg)
{
	char* ret = calloc(strlen(msg)+1, 1);

	for(size_t i = 0, reti = 0; i < strlen(msg); i++)
	{
		if(msg[i] == '\t')
		{
			ret[reti++] = ' ';
		} else if(iscntrl(msg[i])){
			++reti;
		} else if(msg[i] == '#'){
			break;
		} else {
			ret[reti++] = msg[i];
		}
	}

	return ret;
}

char* handle_gtp_cmd(const char* msg, game_state* game)
{
	if(!msg || !strlen(msg))
	{
		char* ret = calloc(1,1);
		return ret;
	}
	char* msg_san = sanitize(msg);

	msg_formatted formatted = format_msg(msg_san);
	char* command = formatted.command;
	Vector* arguments = formatted.arguments;
	char* id = formatted.id;

	char* ret;
	char* (*func_ptr)(const Vector*, const char*, game_state*) = unkown_command_func;

	if(strcmp(command, "protocol_version") == 0)
	{
		func_ptr = protocol_version_func;
	} else if(strcmp(command, "name") == 0) {
		func_ptr = name_func;
	} else if(strcmp(command, "version") == 0) {
		func_ptr = version_func;
	} else if(strcmp(command, "known_command") == 0) {
		func_ptr = known_command_func;
	} else if(strcmp(command, "list_commands") == 0) {
		func_ptr = list_commands_func;
	} else if(strcmp(command, "boardsize") == 0) {
		func_ptr = boardsize_func;
	} else if(strcmp(command, "komi") == 0) {
		func_ptr = komi_func;
	} else if(strcmp(command, "clear_board") == 0) {
		func_ptr = clear_board_func;
	} else if(strcmp(command, "play") == 0) {
		func_ptr = play_func;
	} else if(strcmp(command, "genmove") == 0) {
		func_ptr = genmove_func;
	} else if(strcmp(command, "showboard") == 0) {
		func_ptr = show_board_func;
	} else if(strcmp(command, "final_score") == 0) {
		func_ptr = final_score_func;
	}

	ret = func_ptr(arguments, id, game);

	free(command);
	free(msg_san);
	free(id);
	delete_vector(arguments, free);

	return ret;
}
