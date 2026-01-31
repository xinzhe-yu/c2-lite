CC = gcc
CFLAGS = -Wall -Wextra -g -I common

BUILD_DIR = build

SERVER_TARGET = $(BUILD_DIR)/server
CLIENT_TARGET = $(BUILD_DIR)/client

SERVER_SRCS = server/main.c server/server.c
CLIENT_SRCS = client/main.c client/client.c

SERVER_OBJS = $(SERVER_SRCS:.c=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(SERVER_TARGET): $(SERVER_OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SERVER_OBJS) -o $(SERVER_TARGET)

$(CLIENT_TARGET): $(CLIENT_OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CLIENT_OBJS) -o $(CLIENT_TARGET)

server/%.o: server/%.c server/server.h common/common.h
	$(CC) $(CFLAGS) -c $< -o $@

client/%.o: client/%.c client/client.h common/common.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(SERVER_OBJS) $(CLIENT_OBJS)