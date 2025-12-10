NAME := philo
SRC_FILES :=

OBJ_FILES := $(SRC_FILES:%.c=%.o)
HEADERS := 

CC := cc
C_FLAGS := -g3 -Wall - Wextra -Werror
CPP_FLAGS := -pthread

all : $(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@ $(C_FLAGS)

$(OBJ_FILES):%.o:%.c $(HEADERS)
	$(CC) -c $(C_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_FILES)

fclean:  clean
	rm -rf$(NAME)

re: fclean all

.PHONY: clean fclean all re