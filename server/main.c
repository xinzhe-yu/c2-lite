#include <sys/wait.h>
#include "server.h"
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include "../common/common.h"


int main(){

    int server_fd = socket_create(AF_INET, SOCK_STREAM);
    server_bind(server_fd, AF_INET, PORT);
    server_listen(server_fd, 10);

    while(1){
        int client_fd = server_accept(server_fd);
        
        pid_t pid = fork();
        if (pid == 0){
            close(server_fd);
            session_handle(client_fd);
            exit(0);
        }

        /* Parents continues here */
        // int status; waitpid(-1, &status, WNOHANG);
        waitpid(pid, NULL, 0);
        close(client_fd);
    }

    close(server_fd);
    return 0;
}





























