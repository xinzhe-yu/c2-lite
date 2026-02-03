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
    array->count = 0;
    return array;
}


void client_list_append(client_list_t *list, client_info_t client_data){
    if (list->count > MAX_SESSIONS){
        printf("MAX SESSION REACHED");
        return;
    }
    list->items[list->count] = client_data;
    list->count++;
    printf("\n[*] New session %d from %s:%d\n", list->count - 1, client_data.ip, client_data.port);
}


void client_list_remove(client_list_t *list, int index){
    close(list->items[index].client_fd);
    list->items[index].active = 0;
}

void client_list_print(client_list_t *list){
    for(int i = 0; i < list->count; i++){
        if (list->items[i].active != 0){
            printf("\n[%d] %s:%d\n", i, list->items[i].ip, list->items[i].port);
        }
    }
}


client_info_t* client_list_get(client_list_t *list, int index){
    if (index < list->count){
        return &(list->items[index]);
    }

    return NULL;
    
}


int client_list_count(client_list_t *list){
    return list->count;
}














