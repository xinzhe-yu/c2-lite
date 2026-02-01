#include "client.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h> 
#include <arpa/inet.h> 

int create_socket(int domain, int type){
    int sock_fd = socket(domain, type, 0);
    if(sock_fd < 0){
        return -1;
    }
    return sock_fd;
}

int client_connect(int fd, int domain, uint16_t port, const char* ip){
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = domain;
    server_addr.sin_port = htons(port);
    inet_pton(domain, ip, &server_addr.sin_addr);
    
    if (connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        close(fd);
        return -1;
    }

    return 0;
}

