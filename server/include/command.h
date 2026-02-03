#ifndef COMMAND_H
#define COMMAND_H

#include "server.h"

void cmd_list(client_list_t *client_list);
void cmd_interact(client_list_t *client_list, int id);
void cmd_kill(client_list_t *client_list, int id);
void cmd_dispatch(char *input, client_list_t *client_list);


#endif