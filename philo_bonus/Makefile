# PROJECT DIRECTORIES
SRC_DIR       := src
INC_DIR       := includes
OBJDIR        := objs

# PROJECT SETTINGS
NAME          := philo_bonus
CC            := cc
CFLAGS        := -g -Wall -Wextra -I $(INC_DIR)
THREADS       := -pthread

# SOURCE FILES
SRCS          := \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/meals_utils.c \
	$(SRC_DIR)/parse_input.c \
	$(SRC_DIR)/utils.c \
	$(SRC_DIR)/philo.c

# OBJECT FILES
OBJS          := $(SRCS:$(SRC_DIR)/%.c=$(OBJDIR)/%.o)

# DEFAULT TARGET
all: $(NAME)

# LINKING
$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@ $(THREADS)

# COMPILATION RULE
$(OBJDIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/philo_bonus.h
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# CLEANUP
clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

leaks:
		@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) 4 500 200 214723232 5

qleaks:
		@valgrind -q --leak-check=full ./$(NAME)

hell:
		@valgrind --tool=helgrind ./$(NAME) 4 500 200 214723232 5

norm:
		@norminette | grep Error || true

.PHONY: all clean fclean re leaks qleaks norm
