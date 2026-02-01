CC = gcc
CFLAGS = -Wall -Wextra -I common -g -MMD -MP

BUILD_DIR = build

SERVER_SRCS = $(wildcard server/*.c)
CLIENT_SRCS = $(wildcard client/*.c)

SERVER_OBJS = $(SERVER_SRCS:.c=.o)
CLIENT_OBJS = $(CLIENT_SRCS:.c=.o)

SERVER_DEPS = $(SERVER_OBJS:.o=.d)
CLIENT_DEPS = $(CLIENT_OBJS:.o=.d)

all: $(BUILD_DIR)/server $(BUILD_DIR)/client

$(BUILD_DIR)/server: $(SERVER_OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SERVER_OBJS) -o $@

$(BUILD_DIR)/client: $(CLIENT_OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(CLIENT_OBJS) -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(SERVER_OBJS) $(CLIENT_OBJS) $(SERVER_DEPS) $(CLIENT_DEPS)

-include $(SERVER_DEPS) $(CLIENT_DEPS)