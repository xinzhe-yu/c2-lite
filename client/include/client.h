#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>

int create_socket(int domain, int type);
int client_connect(int fd, int domain, uint16_t port, const char* ip);

#endif