#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "client.h"

#define MAX_CLIENTS 64


static struct {
    client_info_t client_info[MAX_CLIENTS]; 
    int count;
} client_list;

void client_list_init(void){
    memset(&client_list, 0, sizeof(client_list));
    client_list.count = 0;
    
    // Mark all slots as empty
    for (int i = 0; i < MAX_CLIENTS; i++) {
        client_list.client_info[i].client_fd = -1;
    }
}

client_info_t* client_list_append(client_info_t *client_data){
    if (client_list.count >= MAX_CLIENTS){
        printf("MAX SESSION REACHED\n");
        return NULL;
    }

    client_list.client_info[client_list.count] = *client_data;
    client_info_t *entry = &client_list.client_info[client_list.count];
    client_list.count++;

    printf("\n[*] New session %d from %s:%d\n", 
        client_list.count - 1, client_data->ip, client_data->port);
    
    return entry;  // return pointer to the stored entry
}


void client_list_remove(int index){
    if (index < 0 || index >= client_list.count) return;

    close(client_list.client_info[index].client_fd);
    client_list.client_info[index].state = CLIENT_DEAD;
}

void client_list_print(void){
    for(int i = 0; i < client_list.count; i++){
        if (client_is_active(client_list.client_info[i].state)) {
            printf("\n[%d] %s:%d\n", i, 
                client_list.client_info[i].ip, 
                client_list.client_info[i].port);
        }
    }
}


client_info_t* client_info_get_by_index(int index){
    if (index < 0 || index >= client_list.count) return NULL;
    return &(client_list.client_info[index]);
}

client_info_t* client_info_get_by_fd(int fd) {
    for (int i = 0; i < client_list.count; i++) {
        if (client_list.client_info[i].client_fd == fd &&
            client_is_active(client_list.client_info[i].state)) {
            return &client_list.client_info[i];
        }
    }
    return NULL;
}

int client_list_get_count(void){
    return client_list.count;
}














