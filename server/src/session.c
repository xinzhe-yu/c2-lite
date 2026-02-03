#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "session.h"

#define MAX_SESSIONS 16  

struct client_list {
    client_info_t items[MAX_SESSIONS]; 
    int count;
};

client_list_t* client_list_init(void){
    client_list_t *array = malloc(sizeof(client_list_t));
    if (array == NULL) {
        perror("Malloc");
        return NULL;
    }

    array->count = 0;
    return array;
}

void client_list_append(client_list_t *list, client_info_t client_data){
    if (list == NULL) return;

    if (list->count > MAX_SESSIONS){
        printf("MAX SESSION REACHED");
        return;
    }

    list->items[list->count] = client_data;
    list->count++;

    printf("\n[*] New session %d from %s:%d\n", 
        list->count - 1, client_data.ip, client_data.port);
}

void client_list_remove(client_list_t *list, int index){
    if (list == NULL) return;
    if (index < 0 || index >= list->count) return;

    close(list->items[index].client_fd);
    list->items[index].state = CLIENT_DEAD;
}

void client_list_print(client_list_t *list){
    if (list == NULL) return;

    for(int i = 0; i < list->count; i++){
        if (client_is_active(list->items[i].state)) {
            printf("\n[%d] %s:%d\n", i, list->items[i].ip, list->items[i].port);
        }
    }
}


client_info_t* client_list_get(client_list_t *list, int index){
    if (list == NULL) return NULL;
    if (index < 0 || index >= list->count) return NULL;

    return &(list->items[index]);
}


int client_list_count(client_list_t *list){
    if (list == NULL) return 0;
    return list->count;
}














