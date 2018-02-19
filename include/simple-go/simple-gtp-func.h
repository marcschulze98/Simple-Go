#ifndef SIMPLE_GTP_FUNC_H
#define SIMPLE_GTP_FUNC_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <simple-go/simple-go.h>
#include <cutils/cutils.h>

extern const char* const known_commands_string ;
extern const char* known_commands_array[];

char* protocol_version_func(const Vector* arguments, const char* id, game_state* game);
char* name_func(const Vector* arguments, const char* id, game_state* game);
char* version_func(const Vector* arguments, const char* id, game_state* game);
char* known_command_func(const Vector* arguments, const char* id, game_state* game);
char* list_commands_func(const Vector* arguments, const char* id, game_state* game);
char* boardsize_func(const Vector* arguments, const char* id, game_state* game);
char* komi_func(const Vector* arguments, const char* id, game_state* game);
char* clear_board_func(const Vector* arguments, const char* id, game_state* game);
char* play_func(const Vector* arguments, const char* id, game_state* game);
char* genmove_func(const Vector* arguments, const char* id, game_state* game);
char* show_board_func(const Vector* arguments, const char* id, game_state* game);
char* final_score_func(const Vector* arguments, const char* id, game_state* game);



#endif //SIMPLE_GTP_FUNC_H
