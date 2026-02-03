#ifndef SESSION_H
#define SESSION_H

#include <stdint.h>
#include <netinet/in.h>



typedef struct client_list client_list_t; 

/* Function Declearation */
typedef struct{
    int client_fd;
    char ip[INET_ADDRSTRLEN];
    uint16_t port;
    int active;
} client_info_t;


client_list_t* client_list_init(void);
void client_list_append(client_list_t *list, client_info_t client_data);
void client_list_remove(client_list_t *list, int index);


client_info_t* client_list_get(client_list_t *list, int index);
int client_list_count(client_list_t *list);






#endif

