#include "client.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void cmd_list(client_list_t *client_list){
    client_list_print(client_list);
}

void cmd_interact(client_list_t *client_list, int id){
    if (client_list == NULL) return;
    
    if(id < 0 || id >= client_list_count(client_list) || client_is_gone(client_list_get(client_list, id)->state)){
        printf("Invalid Session");
        return;
    }

    printf("\n%s:%d\n", client_list_get(client_list, id)->ip, client_list_get(client_list, id)->port);
    client_handle(client_list_get(client_list, id)->client_fd);
}

void cmd_kill(client_list_t *client_list, int id){
    if (client_list == NULL) return;

    if(id < 0 || id >= client_list_count(client_list) || client_is_gone(client_list_get(client_list, id)->state)){
        printf("Invalid Session");
        return;
    }
    client_list_remove(client_list, id);
}

void cmd_dispatch(char *input, client_list_t *client_list){
    char *saveptr;
    char *cmd = strtok_r(input, " \n", &saveptr);
    if (client_list == NULL) return;
    if (cmd == NULL) return;

    if (strcmp(cmd, "list") == 0) {
        cmd_list(client_list);
    }
    else if (strcmp(cmd, "interact") == 0) {
        char *arg = strtok_r(NULL, " \n", &saveptr);
        if (arg == NULL) {
            printf("Usage: interact <id>\n");
            return;
        }
        int id = atoi(arg);
        cmd_interact(client_list, id);
    }

    else if (strcmp(cmd, "kill") == 0) {
        char *arg = strtok_r(NULL, " \n", &saveptr);
        if (arg == NULL) {
            printf("Usage: kill <id>\n");
            return;
        }
        int id = atoi(arg);
        cmd_kill(client_list, id);
    }
    else {
        printf("Unknown command: %s\n", cmd);
    }
}























