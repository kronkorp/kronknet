##
## EPITECH PROJECT, 2026
## KRONKNET
## File description:
## Makefile for kronknet tests
##

SSRC = 	chat_server.c \

CSRC = 	chat_client.c \

LDFLAGS += -L../../Debug -lkronknet -Wl,-rpath=../../Debug
CFLAGS += -I../../include/ -g

COBJ = $(CSRC:.c=.o)
SOBJ = $(SSRC:.c=.o)

CC = gcc

SNAME = ex_chat
CNAME = ex_chater

NAME = chat

all: $(NAME)

$(NAME): $(SOBJ) $(COBJ)
	$(CC) $(CFLAGS) -o $(SNAME) $(SOBJ) $(LDFLAGS)
	$(CC) $(CFLAGS) -o $(CNAME) $(COBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
