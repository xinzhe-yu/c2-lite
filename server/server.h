
#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>
#include <netinet/in.h>

/* Function Declearation */
typedef struct{
    int client_fd;
    char ip[INET_ADDRSTRLEN];
    uint16_t port;
    int active;
} session_info;

int socket_create(int domain, int type);
void server_bind(int fd, int domain, uint16_t port);
void server_listen(int fd, int backlog); 
int server_accept(int fd, session_info *session);
void session_handle(int fd);

#endif
























