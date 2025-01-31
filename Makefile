# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vvaucoul <vvaucoul@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/30 09:10:36 by vvaucoul          #+#    #+#              #
#    Updated: 2025/01/30 21:14:31 by vvaucoul         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#═══════════════════════════════════════════════════════════════════════════════#
#                                   COLORS                                       #
#═══════════════════════════════════════════════════════════════════════════════#
RED    = \033[0;31m
GREEN  = \033[0;32m
YELLOW = \033[0;33m
BLUE   = \033[0;34m
PURPLE = \033[0;35m
CYAN   = \033[0;36m
WHITE  = \033[0;37m
RESET  = \033[0m

#═══════════════════════════════════════════════════════════════════════════════#
#                                PROJECT SETUP                                   #
#═══════════════════════════════════════════════════════════════════════════════#
NAME        = libhypercore.so
CC          = gcc
CFLAGS      = -Wall -Wextra -std=c2x -fPIC #-Werror 

#═══════════════════════════════════════════════════════════════════════════════#
#                                DIRECTORIES                                     #
#═══════════════════════════════════════════════════════════════════════════════#
SRC_DIR     = src
INC_DIR     = includes
OBJ_DIR     = obj
TEST_DIR    = tests

#═══════════════════════════════════════════════════════════════════════════════#
#                                  SOURCES                                       #
#═══════════════════════════════════════════════════════════════════════════════#
SRCS        = $(shell find $(SRC_DIR) -name '*.c')
OBJS        = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
CFLAGS     += -I$(INC_DIR)

#═══════════════════════════════════════════════════════════════════════════════#
#                                   TESTS                                        #
#═══════════════════════════════════════════════════════════════════════════════#
TEST_SRCS   = $(shell find $(TEST_DIR) -name '*.c')
TEST_OBJS   = $(TEST_SRCS:$(TEST_DIR)/%.c=$(OBJ_DIR)/%.o)
TEST_NAME   = run_tests

#═══════════════════════════════════════════════════════════════════════════════#
#                                   RULES                                        #
#═══════════════════════════════════════════════════════════════════════════════#
all: banner $(NAME)

banner:
	@echo "$(CYAN)"
	@echo "╔═══════════════════════════════════════════════════════════════════╗"
	@echo "║                     Building HyperCore Library                    ║"
	@echo "╚═══════════════════════════════════════════════════════════════════╝"
	@echo "$(RESET)"

$(NAME): $(OBJS)
	@echo "$(BLUE)► Linking objects into shared library [$(NAME)]...$(RESET)"
	@$(CC) -shared -o $(NAME) $(OBJS)
	@echo "$(GREEN)✓ Successfully built $(NAME)!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)► Compiling: $<$(RESET)"
	@$(CC) $(CFLAGS) -c $< -o $@

test: $(NAME) $(TEST_OBJS)
	@echo "$(BLUE)► Building test suite...$(RESET)"
	@$(CC) $(CFLAGS) $(TEST_OBJS) $(NAME) -o $(TEST_NAME)
	@echo "$(GREEN)► Executing test suite:$(RESET)"
	@./$(TEST_NAME)

install: $(NAME)
	@echo "$(BLUE)► Installing $(NAME) to system...$(RESET)"
	@sudo cp $(NAME) /usr/local/lib/
	@sudo ldconfig
	@echo "$(GREEN)✓ Successfully installed $(NAME) to system!$(RESET)"

uninstall:
	@echo "$(RED)► Uninstalling $(NAME) from system...$(RESET)"
	@sudo rm -f /usr/local/lib/$(NAME)
	@sudo ldconfig
	@echo "$(GREEN)✓ Successfully uninstalled $(NAME) from system!$(RESET)"

clean:
	@echo "$(RED)► Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@rm -f $(TEST_NAME)
	@echo "$(GREEN)✓ Cleaned object files$(RESET)"

fclean: clean
	@echo "$(RED)► Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@echo "$(GREEN)✓ Removed $(NAME)$(RESET)"

re: fclean all

.PHONY: all clean fclean re test install banner
