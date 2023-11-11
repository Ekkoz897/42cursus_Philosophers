NAME = philo

CC = cc

RM = rm -rf

CFLAGS = -Wall -Wextra -Werror #-g fsanitize=address

SRC = main.c init.c utils.c utils2.c utils3.c police.c routines.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(addprefix SRC/,$(OBJ))
	@$(CC) $(addprefix SRC/,$(OBJ)) -pthread -o $(NAME)

run: $(NAME)
	@./philo 5 8000 200 200 7

valgrind: $(NAME)
	@make re && valgrind --leak-check=full --show-leak-kinds=all ./philo 5 8000 200 200 7

git: fclean
	@git add .
	@git commit
	@git push
	@clear
	@echo "                                                  "
	@echo "                                                  "
	@echo "         -------Commited and Pushed-------        "
	@echo "                                                  "
	@echo "                                                  "

commit: fclean

clean:
	@$(RM) $(addprefix SRC/,$(OBJ))

fclean: clean
	@$(RM) $(NAME)

re: fclean all