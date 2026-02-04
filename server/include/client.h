#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>
#include <netinet/in.h>
#include <stdbool.h>

typedef struct client_list client_list_t; 

typedef struct {
    int client_fd;
    char ip[INET_ADDRSTRLEN];
    uint16_t port;
    client_state_t state;
    uint32_t client_id;
    time_t connect_time;
    time_t last_seen;

} client_info_t;

typedef enum {
    CLIENT_ALIVE = 0,    // checked in recently
    CLIENT_TASKED,       // has pending commands
    CLIENT_RESPONDING,   // sending data back
    CLIENT_UNKNOWN,
    CLIENT_DORMANT,      // missed check-ins but not dead
    CLIENT_KILLED,       // Manually Killed
    CLIENT_DEAD          // presumed lost
} client_state_t;

static inline bool client_is_active(client_state_t s){
    return s <= CLIENT_RESPONDING;
}

static inline bool client_is_gone (client_state_t s){
    return s >= CLIENT_KILLED;
}

client_list_t* client_list_init(void);
void client_list_append(client_list_t *list, client_info_t client_data);
void client_list_remove(client_list_t *list, int index);
void client_list_print(client_list_t *list);

client_info_t* client_list_get(client_list_t *list, int index);
int client_list_count(client_list_t *list);



#endif

