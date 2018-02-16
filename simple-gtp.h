#ifndef SIMPLE_GTP_H
#define SIMPLE_GTP_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "simple-go.h"
#include "cutils.h"

char* handle_gtp_cmd(const char* msg, game_state* game);

#endif //SIMPLE_GTP_H
