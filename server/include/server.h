
#ifndef SERVER_H
#define SERVER_H



#include "session.h"


int socket_create(int domain, int type);
void server_bind(int fd, int domain, uint16_t port);
void server_listen(int fd, int backlog); 
int server_accept(int fd, client_info_t *client);
void client_handle(int fd);

#endif
























