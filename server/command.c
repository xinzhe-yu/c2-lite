

#include "command.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void cmd_list(session_info *session_list, int length){
    for(int i = 0; i < length; i++){
        if (session_list[i].active){
            printf("\n[%d] %s:%d\n", i, session_list[i].ip, session_list[i].port);
        }
    }
}

void cmd_interact(session_info *session_list, int length, int id){
    if(id < 0 || id >= length || session_list[id].active == 0){
        printf("Invalid Session");
        return;
    }

    printf("\n%s:%d\n", session_list[id].ip, session_list[id].port);
    session_handle(session_list[id].client_fd);
}



void cmd_kill(session_info *session_list, int length, int id){
    if(id < 0 || id >= length || session_list[id].active == 0){
        printf("Invalid Session");
        return;
    }

    session_list[id].active = 0;
    close(session_list[id].client_fd);
}




void cmd_dispatch(char *input, session_info *session_list, int length){
    char *saveptr;
    char *cmd = strtok_r(input, " \n", &saveptr);
    if (cmd == NULL) return;

    
    if (strcmp(cmd, "list") == 0) {
        cmd_list(session_list, length);
    }
    else if (strcmp(cmd, "interact") == 0) {
        char *arg = strtok_r(NULL, " \n", &saveptr);
        if (arg == NULL) {
            printf("Usage: interact <id>\n");
            return;
        }
        int id = atoi(arg);
        cmd_interact(session_list, length, id);
    }
    else if (strcmp(cmd, "kill") == 0) {
        char *arg = strtok_r(NULL, " \n", &saveptr);
        if (arg == NULL) {
            printf("Usage: kill <id>\n");
            return;
        }
        int id = atoi(arg);
        cmd_kill(session_list, length, id);
    }
    else {
        printf("Unknown command: %s\n", cmd);
    }
}























