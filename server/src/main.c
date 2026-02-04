#include <sys/wait.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/select.h>
#include <stdio.h>
#include <sys/epoll.h>

#include "../common/common.h"
#include "server.h"
#include "command.h"
#include "terminal.h"
#include "client.h"
#include "net_loop.h"

int main(){

    int server_fd; //make this static inside API and have an getter for EPOLL

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

    loop_init();
    loop_add_fd(server_fd, EPOLLIN, handle_accept, client_list);
    loop_add_fd(STDIN_FILENO, EPOLLIN, handle_stdin, client_list);
    

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
            

        }

        /* Watches STDIN */
        if (FD_ISSET(STDIN_FILENO, &readfds)) {
            
        }

        //Still hasnt fixed select.
    }

    close(server_fd);
    return 0;
}


























