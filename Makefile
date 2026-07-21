# ---- Project settings ----
TARGET    := HttpServer
SRC_DIR   := src
INC_DIR   := include
BUILD_DIR := build

# Recursively find all .c files anywhere under src/ (fs, http, net, util, ...)
SRCS := $(shell find $(SRC_DIR) -name '*.c')

# Mirror src/ tree into build/ for object files, e.g. src/net/server.c -> build/net/server.o
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))

# ---- Compiler settings ----
CC      := gcc
CFLAGS  := -Wall -Wextra -I$(INC_DIR) -Ilexer -Iparser -pthread
LDFLAGS := 

# ---- Targets ----
.PHONY: all run clean rebuild

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	@./$(BUILD_DIR)/$(TARGET)

clean:
	@echo "CLEANED"
	@rm -rf $(BUILD_DIR)

rebuild: clean all