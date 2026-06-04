##
## EPITECH PROJECT, 2026
## KRONKNET
## File description:
## Makefile for kronknet tests
##

SRC = 	hangman.c

LDFLAGS += -L../../Debug -lkronknet
CFLAGS += -I../../include/

OBJ = $(SRC:.c=.o)

CC = gcc

NAME = ex_hangman

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
