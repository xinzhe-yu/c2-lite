CC = gcc
CFLAGS = -Wall -Wextra -g -MMD -MP

BUILD_DIR = build

# Include paths for both client and server headers
SERVER_INCLUDES = -I./server/include -I./common
CLIENT_INCLUDES = -I./client/include -I./common

# Source files
SERVER_SRCS = $(wildcard server/src/*.c)
CLIENT_SRCS = $(wildcard client/src/*.c)

# Object files go in build directory
SERVER_OBJS = $(patsubst server/src/%.c,$(BUILD_DIR)/server/%.o,$(SERVER_SRCS))
CLIENT_OBJS = $(patsubst client/src/%.c,$(BUILD_DIR)/client/%.o,$(CLIENT_SRCS))

# Dependency files
SERVER_DEPS = $(SERVER_OBJS:.o=.d)
CLIENT_DEPS = $(CLIENT_OBJS:.o=.d)

# Directory targets
SERVER_BUILD_DIR = $(BUILD_DIR)/server
CLIENT_BUILD_DIR = $(BUILD_DIR)/client

all: $(BUILD_DIR)/server_bin $(BUILD_DIR)/client_bin

$(BUILD_DIR)/server_bin: $(SERVER_OBJS)
	$(CC) $(SERVER_OBJS) -o $@

$(BUILD_DIR)/client_bin: $(CLIENT_OBJS)
	$(CC) $(CLIENT_OBJS) -o $@

# Create build directories
$(SERVER_BUILD_DIR):
	mkdir -p $@

$(CLIENT_BUILD_DIR):
	mkdir -p $@

# Server object compilation
$(BUILD_DIR)/server/%.o: server/src/%.c | $(SERVER_BUILD_DIR)
	$(CC) $(CFLAGS) $(SERVER_INCLUDES) -c $< -o $@

# Client object compilation
$(BUILD_DIR)/client/%.o: client/src/%.c | $(CLIENT_BUILD_DIR)
	$(CC) $(CFLAGS) $(CLIENT_INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean

-include $(SERVER_DEPS) $(CLIENT_DEPS)