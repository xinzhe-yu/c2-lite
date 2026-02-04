#include <sys/epoll.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
//#include <fcntl.h>

#include "net_loop.h"
#include "client.h"
#include "server.h"
#include "command.h"

#define MAX_FDS 1024

typedef struct {
    int fd;
    fd_callback_t cb;
    void *ctx; // pass in client_info_t for context
} fd_entry_t;

static struct{
    int epfd;
    int running;
    fd_entry_t entries[MAX_FDS];
} loop_state = {
    .epfd = -1,
    .running = 0,
};


void loop_init(void){
    if(loop_state.epfd != -1){
        close(loop_state.epfd);
    }

    loop_state.epfd = epoll_create1(EPOLL_CLOEXEC);
    if(loop_state.epfd < 0) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MAX_FDS; i++) {
        loop_state.entries[i].fd = -1;
    }

    loop_state.running = 1;
}

int loop_add_fd(int fd, uint32_t events, fd_callback_t cb, void *ctx){
    if (fd < 0 || fd >= MAX_FDS) return -1;

    int slot = -1;
    for (int i = 0; i < MAX_FDS; i++){
        if (loop_state.entries[i].fd == -1){
            slot = i;
            break;
        }
    }

    /* IF FDS FULL */
    if (slot < 0) return -1;

    loop_state.entries[slot].fd = fd;
    loop_state.entries[slot].cb = cb;
    loop_state.entries[slot].ctx = ctx;

    /* Set up data struct for passing */
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = events;
    ev.data.fd = fd;

    // Pass to kernal
    if (epoll_ctl(loop_state.epfd, EPOLL_CTL_ADD, fd, &ev) < 0) {
        perror("epoll_ctl ADD");
        memset(&loop_state.entries[slot], 0, sizeof(loop_state.entries[slot]));
        return -1;
    }

    return 0;
}

void loop_run(void){
    struct epoll_event events[MAX_FDS];

    while (loop_state.running){
        int nfds = epoll_wait(loop_state.epfd, events, MAX_FDS, -1);

        if (nfds < 0) {
            if (errno == EINTR) continue;
            perror("epoll_wait");
            break;
        }

        for (int i = 0; i < nfds; i++) {
            int current_fd = events[i].data.fd;
            uint32_t revents = events[i].events;

            for (int j = 0; j < MAX_FDS; j++){
                if (loop_state.entries[j].fd == current_fd){
                    loop_state.entries[j].cb(current_fd, revents, loop_state.entries[j].ctx);
                }
            }
        }
    }
}

int loop_remove_fd(int client_fd){
    if (client_fd < 0 || client_fd >= MAX_FDS) return -1;

    if(epoll_ctl(loop_state.epfd, EPOLL_CTL_DEL, client_fd, NULL) < 0) {
        perror("epoll_ctl DEL");
        return -1;
    }

    for (int i = 0; i < MAX_FDS; i++) {
        if (loop_state.entries[i].fd == client_fd){
            memset(&loop_state.entries[i], 0, sizeof(loop_state.entries[i]));
            loop_state.entries[i].fd = -1;
        }
    }

    return 0;
}

void loop_stop(void){
    loop_state.running = 0;
    if (loop_state.epfd >= 0){
        close(loop_state.epfd);
        loop_state.epfd = -1;
    }
}

// I need to make the CB functions for each senarios 


void handle_accept(int fd, uint32_t events, void *ctx){
    // new client connecting
    client_info_t client_data;
    if(server_accept(fd, &client_data) < 0){
        perror("socket accept");
    }
    
    client_info_t *info = client_list_append(&client_data);
    if (info == NULL) return;

    loop_add_fd(client_data.client_fd, EPOLLIN, handle_client, info);
}

void handle_client(int fd, uint32_t events, void *ctx){
    client_info_t *client_info = (client_info_t *)ctx;
    
    // Handle disconnect
    if (events & (EPOLLHUP | EPOLLERR)) {
        printf("\n[!] Client %s:%d disconnected\n", client_info->ip, client_info->port);
        loop_remove_fd(fd);
        close(fd);
        client_info->state = CLIENT_DEAD;
        return;
    }
    
    // Data available - drain it for now (we're not in interact mode)
    if (events & EPOLLIN) {
        char buf[4096];
        ssize_t n = read(fd, buf, sizeof(buf));
        
        if (n <= 0) {
            // Client closed or error
            printf("\n[!] Client %s:%d disconnected\n", client_info->ip, client_info->port);
            loop_remove_fd(fd);
            close(fd);
            client_info->state = CLIENT_DEAD;
            return;
        }
        
        // For now: discard the data (no interact mode yet)
        // Later: buffer it or check if we're interacting
    }

}

void handle_stdin(int fd, uint32_t events, void *ctx){
    // operator typed a command
    char cmd[256];
    ssize_t n = read(fd, cmd, sizeof(cmd) - 1);
    if (n <= 0) return;
    cmd[n] = '\0';
    
    /* Parse command here */
    cmd_dispatch(cmd);
}



















