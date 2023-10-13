NAME = philo

CC = cc

RM = rm -rf

CFLAGS = -Wall -Wextra -Werror #-g fsanitize=address

SRC = main.c utils.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(addprefix SRC/,$(OBJ))
	@$(CC) -g $(addprefix SRC/,$(OBJ)) -pthread -o $(NAME)

run: $(NAME)
	@./philo 10 80 80 80 2

valgrind: $(NAME)
	@make re && valgrind --leak-check=full --show-leak-kinds=all ./philo 10 80 80 80 2

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