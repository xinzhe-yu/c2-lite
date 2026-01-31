#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <stdbool.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1" 

int main(){

    while(1){
    
    //Socket
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_fd < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in server_addr; //server address to connect to 
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    //Convert IP string to binary
    //server_addr.sin_addr.s_addr = htonl()
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    printf("Searching for server\n");
    
    //Connect
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
        perror("Connect Failed");
        close(client_fd);
        sleep(5);
        continue;
    }
    
    

    printf("Server Connected \n");
    //send()
    char *msg = "POST / HTTP/1.1\r\nHost: localhost\r\nContent-Length: 13\r\n\r\nHello Server!";
    size_t s_total = strlen(msg);
    size_t sent = 0;
    while(sent < s_total){
        ssize_t n = send(client_fd, msg + sent, s_total - sent, 0);
        if (n < 0){ perror("Sent"); break;}
        sent += n;
    }
    

    //recv() dynamic
    size_t capacity = 1024; 
    size_t received = 0;
    char *buffer = malloc(sizeof(char) * capacity);
    if(!buffer){
        perror("malloc");
        //free(buffer);
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    ssize_t n = 0;
    while((n = recv(client_fd, buffer + received, capacity - received - 1, 0)) > 0){
        received += n;
        buffer[received] = '\0';

        //Check for end of HTTP header and break
        if (strstr(buffer, "\r\n\r\n") != NULL) {
            break;  // Got full request, stop reading; bug for real http 
        }

        //Realloc for more space
        if (received >= capacity -1 ){ // -1 to leave room for null terminator
            size_t new_capacity = capacity * 2;

            //Overflow check for large allocations
            if (new_capacity < capacity){
                fprintf(stderr, "Integer overflow in buffer resize\n");
                close(client_fd);
                free(buffer);
                exit(EXIT_FAILURE);
            }

            char *temp = realloc(buffer, new_capacity);
            if(!temp){
                perror("realloc");
                close(client_fd);
                free(buffer);
                exit(EXIT_FAILURE);
            }

            buffer = temp;
            capacity = new_capacity;
        }
        
    }

    if (n < 0) {
        perror("recv");
    }

    buffer[received] = '\0';

    printf("Received:\n%s\n", buffer);


    
    //clean up
    free(buffer);
    close(client_fd);

    sleep(3);
    }


    return 0;
}




















