#ifndef CLIENT_H
#define CLIENT_H

#include <stdint.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <time.h>


typedef enum {
    CLIENT_ALIVE = 0,    // checked in recently
    CLIENT_TASKED,       // has pending commands
    CLIENT_RESPONDING,   // sending data back
    CLIENT_UNKNOWN,
    CLIENT_DORMANT,      // missed check-ins but not dead
    CLIENT_KILLED,       // Manually Killed
    CLIENT_DEAD          // presumed lost
} client_state_t;

typedef struct {
    int client_fd;
    char ip[INET_ADDRSTRLEN];
    uint16_t port;
    client_state_t state;
    uint32_t client_id;
    time_t connect_time;
    time_t last_seen;

} client_info_t;


static inline bool client_is_active(client_state_t s){
    return s <= CLIENT_RESPONDING;
}

static inline bool client_is_gone (client_state_t s){
    return s >= CLIENT_KILLED;
}

void client_list_init(void);
client_info_t* client_list_append(client_info_t *client_data);
void client_list_remove(int index);
void client_list_print(void);

client_info_t* client_info_get_by_index(int index);
client_info_t* client_info_get_by_fd(int fd);
int client_list_get_count(void);



#endif

