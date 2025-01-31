# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/30 09:58:38 by vvaucoul          #+#    #+#              #
#    Updated: 2025/01/31 10:22:16 by vvaucoul         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# =============================================================================
# Project Configuration
# =============================================================================

NAME        := tests
CC          := gcc
CFLAGS      := -Wall -Wextra -Werror -g3

# =============================================================================
# Directory Structure
# =============================================================================

SRC_DIR     := src
OBJ_DIR     := obj
BIN_DIR     := bin

# =============================================================================
# Libraries and Includes
# =============================================================================

INCLUDES    := -I../includes
LIBS        := -L.. -lhypercore
RPATH       := -Wl,-rpath,$(realpath ..)

# =============================================================================
# Source and Object Files
# =============================================================================

SRCS        := $(shell find $(SRC_DIR) -type f -name '*.c')
OBJS        := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BINS        := $(SRCS:$(SRC_DIR)/%.c=$(BIN_DIR)/%)

# =============================================================================
# Build Rules
# =============================================================================

all: build_lib $(OBJ_DIR) $(BIN_DIR) $(BINS)
	@echo "\033[32m✓ Build complete: all binaries\033[0m"

# Build the library first
build_lib:
	@echo "\033[33mBuilding library...\033[0m"
	@make -C ../. -s

# Run all tests
test_all: all
	@echo "\033[33mRunning all tests...\033[0m"
	@for test in $(BINS); do \
		echo "\033[33mRunning: $$test\033[0m"; \
		$$test; \
	done

# Create object directory
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@echo "\033[34mCreated directory: $(OBJ_DIR)\033[0m"

# Create binary directory
$(BIN_DIR):
	@mkdir -p $(BIN_DIR)
	@echo "\033[34mCreated directory: $(BIN_DIR)\033[0m"

# Compile and link each binary
$(BIN_DIR)/%: $(OBJ_DIR)/%.o
	@mkdir -p $(dir $@)
	@echo "\033[33mLinking: $@\033[0m"
	@$(CC) $(CFLAGS) $< -o $@ $(INCLUDES) $(LIBS) $(RPATH)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "\033[36mCompiling: $<\033[0m"
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

# =============================================================================
# Cleanup Rules
# =============================================================================

clean:
	@rm -rf $(OBJ_DIR)
	@echo "\033[35mCleaned object files\033[0m"

fclean: clean
	@rm -rf $(BIN_DIR)
	@make -C ../. fclean -s
	@echo "\033[35mCleaned binaries\033[0m"

re: fclean all

# =============================================================================
# Special Targets
# =============================================================================

.PHONY: all clean fclean re
