NAME := philo
SRC_FILES := main.c philos_routine.c readd_write_stop.c validate_input.c

OBJ_FILES := $(SRC_FILES:%.c=%.o)
HEADERS := philosophers.h

CC := cc
C_FLAGS := -g3 -Wall -Wextra -Werror
CPP_FLAGS := -pthread

all : $(NAME)

$(NAME): $(OBJ_FILES)
	$(CC) $(OBJ_FILES) -o $@ $(C_FLAGS)

$(OBJ_FILES):%.o:%.c $(HEADERS)
	$(CC) -c $(CPP_FLAGS) $(C_FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_FILES)

fclean:  clean
	rm -rf$(NAME)

re: fclean all

.PHONY: clean fclean all re