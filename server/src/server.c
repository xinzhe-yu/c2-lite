
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/select.h>
#include "server.h"
#include <unistd.h>
#include <arpa/inet.h>

#define CTRL_Z   0x1a  

int socket_create(int domain, int type, int *fd){
    int socket_fd = socket(domain, type, 0);
    if (socket_fd < 0) {
        return -1;
    }

    int opt = 1; 
    setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    *fd = socket_fd;
    return 0;
}


int server_bind(int fd, int domain, uint16_t port){
    /* Structure used for IPv4 socket addressing */
    struct sockaddr_in server_addr; 
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = domain;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){  
        return -1;
    }
    return 0;
}


int server_listen(int fd, int backlog){
    if(listen(fd, backlog) < 0){
        return -1;
    }
    printf("Waiting for connection...\n\n");
    return 0;
}



int server_accept(int fd, client_info_t *client){
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(client_addr));
    socklen_t client_addr_len = sizeof(client_addr);

    /* Blocks until connect */
    int client_fd = accept(fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_fd < 0){
        perror("accept");
        close(fd);
        return -1;
    }
    printf("Client connected!\n"); 

    inet_ntop(AF_INET, &client_addr.sin_addr, client->ip, INET_ADDRSTRLEN);
    client->port = ntohs(client_addr.sin_port);
    client->client_fd = client_fd;
    client->active = 1;

    return 0;
}


void client_handle(int fd){
    char text[] = "$ ";
    write(STDOUT_FILENO, text, sizeof(text));

    while(1){ 
        /* Create fresh fd_sets */
        fd_set readfds;
        FD_ZERO(&readfds);

        /* Add Fds to watch */
        FD_SET(STDIN_FILENO, &readfds);
        FD_SET(fd, &readfds);

        /* Find max Fd */
        int maxfd = fd;

        /* Block until something is ready */
        int ready = select(maxfd + 1, &readfds, NULL, NULL, NULL);

        if (ready < 0) {
            perror("Select");
            break;
        }

        /* Check which Fds are ready */
        if (FD_ISSET(STDIN_FILENO, &readfds)){
            /* read from terminal */
            char buf[4096];
            ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
            if (n <= 0) break;

            if (n == 1){
                switch(buf[0]) {
            
                    case CTRL_Z:
                        /* Background */
                        printf("\n[*] Backgrounding session\n");
                        return;
                    /* Room for expansion */
                }
            }

            /* send to client */
            write(fd, buf, n);
        }

        if(FD_ISSET(fd, &readfds)){
            char buf[4096];
            ssize_t n = read(fd, buf, sizeof(buf));
            if (n <= 0) break;
            write(STDOUT_FILENO, buf, n);

            char text[] = "$ ";
            write(STDOUT_FILENO, text, sizeof(text));
        }
    }
}



























