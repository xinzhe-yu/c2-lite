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

    int server_fd = socket_setup(PORT);
    client_list_init();

    loop_init();
    loop_add_fd(server_fd, EPOLLIN, handle_accept, NULL); /* Watches incoming connection */
    loop_add_fd(STDIN_FILENO, EPOLLIN, handle_stdin, NULL); /* Watches STDIN */
    loop_run();
 
    close(server_fd);
    return 0;
}


























