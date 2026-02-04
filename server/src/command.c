#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "client.h"

//delete me
#include "server.h"


void cmd_list(void){
    client_list_print();
}

void cmd_interact(int id){ 
    if(id < 0 || id >=  client_list_get_count() || client_is_gone(client_info_get_by_index(id)->state)){
        printf("Invalid Session");
        return;
    }

    printf("\n%s:%d\n", client_info_get_by_index(id)->ip, client_info_get_by_index(id)->port);
    client_handle(client_info_get_by_index(id)->client_fd);
}

void cmd_kill(int id){
    if(id < 0 || id >=  client_list_get_count() || client_is_gone(client_info_get_by_index(id)->state)){
        printf("Invalid Session");
        return;
    }
    client_list_remove( id);
}

void cmd_dispatch(char *input){
    char *saveptr;
    char *cmd = strtok_r(input, " \n", &saveptr);

    if (cmd == NULL) return;

    // List
    if (strcmp(cmd, "list") == 0) {
        cmd_list();
    }
    // Interact
    else if (strcmp(cmd, "interact") == 0) {
        char *arg = strtok_r(NULL, " \n", &saveptr);
        if (arg == NULL) {
            printf("Usage: interact <id>\n");
            return;
        }
        int id = atoi(arg);
        cmd_interact( id);
    }
    // Kill
    else if (strcmp(cmd, "kill") == 0) {
        char *arg = strtok_r(NULL, " \n", &saveptr);
        if (arg == NULL) {
            printf("Usage: kill <id>\n");
            return;
        }
        int id = atoi(arg);
        cmd_kill( id);
    }
    // Unknown
    else {
        printf("Unknown command: %s\n", cmd);
    }
}























