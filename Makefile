# Compiler and flags (override on the command line, e.g. `make CC=clang`)
CC      ?= cc
CFLAGS  ?= -Wall -Wextra -ggdb -Wswitch-enum
LDFLAGS ?=
LDLIBS  ?=

SRC_DIR   := src
BUILD_DIR := build
TARGET    := $(BUILD_DIR)/parser

SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)

run: $(TARGET)
	./$(BUILD_DIR)/parser

-include $(DEPS)
