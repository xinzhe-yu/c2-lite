#ifndef COMMAND_H
#define COMMAND_H

#include "server.h"

void cmd_list(session_info *session_list, int length);
void cmd_interact(session_info *session_list, int length, int id);
void cmd_kill(session_info *session_list, int length, int id);
void cmd_dispatch(char *input, session_info *session_list, int length);


#endif