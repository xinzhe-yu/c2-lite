


#include "session.h"

#include <stdio.h>
#include <stdlib.h>



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

    list->items[list->count] = client_data;
    list->count++;

    printf("\n[*] New session %d from %s:%d\n", list->count - 1, client_data.ip, client_data.port);

}


void client_list_remove(client_list_t *list, int index){

    //list->items[index] 
    close(list->items[index].client_fd);
    list->items[index].active = 0;



}


client_info_t* client_list_get(client_list_t *list, int index){
    return &(list->items[index]);

}
int client_list_count(client_list_t *list){
    return list->count;
}














