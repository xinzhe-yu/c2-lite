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
#include "terminal.h"
#include "session.h"


int main(){

    int server_fd;

    if(socket_create(AF_INET, SOCK_STREAM, &server_fd) < 0){
        perror("Socket");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if(server_bind(server_fd, AF_INET, PORT) < 0){
        perror("Bind");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if(server_listen(server_fd, 10) < 0 ){
        perror("Listen");
        exit(EXIT_FAILURE);
    }
    
    /* Client list array will store client_data struct*/
    client_list_t *client_list = client_list_init();
    
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

        /* Watches incoming connection */
        if (FD_ISSET(server_fd, &readfds)) {
            // new client connecting
            client_info_t client_data;
            if(server_accept(server_fd, &client_data) < 0) continue;
            client_list_append(client_list, client_data);

        }

        /* Watches STDIN */
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            // operator typed a command
            char cmd[256];
            ssize_t n = read(STDIN_FILENO, cmd, sizeof(cmd) - 1);
            if (n <= 0) break;
            cmd[n] = '\0';
            
            /* Parse command here */
            cmd_dispatch(cmd, client_list);
        }

        //Still hasnt fixed select.
    }

    close(server_fd);
    return 0;
}


























