#ifndef COMMAND_H
#define COMMAND_H

#include "server.h"

void cmd_list(void);
void cmd_interact(int id);
void cmd_kill(int id);
void cmd_dispatch(char *input);


#endif