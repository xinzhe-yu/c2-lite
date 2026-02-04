#ifndef NET_LOOP_H
#define NET_LOOP_H

#include <stdint.h>


typedef void (*fd_callback_t)(int fd, uint32_t events, void *ctx);

/* API */
void loop_init(void);
int  loop_add_fd(int client_fd, uint32_t events, fd_callback_t cb, void *ctx);
int  loop_remove_fd(int client_fd);
void loop_run(void);
void loop_stop(void);

/* Call Back Functions*/
void handle_accept(int fd, uint32_t events, void *ctx);
void handle_stdin(int fd, uint32_t events, void *ctx);
void handle_client(int fd, uint32_t events, void *ctx);

#endif