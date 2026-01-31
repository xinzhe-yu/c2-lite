
#ifndef SERVER_H
#define SERVER_H

#include <stdint.h>

/* Function Declearation */
int socket_create(int domain, int type);
void server_bind(int fd, int domain, uint16_t port);
void server_listen(int fd, int backlog); 
int server_accept(int fd);
void session_handle(int fd);

#endif
























