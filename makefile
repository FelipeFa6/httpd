# See LICENSE file for copyright and license details.
NAME = httpd
LOCATION = /usr/local/bin/
CC = cc
CFLAGS = -Wall -Werror -g -pedantic

SRC = httpd.c

OBJ = $(SRC:.c=.o)


all: $(NAME)

httpd: $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: clean all
	mkdir -p $(LOCATION)
	cp $(NAME) $(LOCATION)

uninstall:
	rm $(LOCATION)$(NAME)

clean:
	rm -f $(OBJ) $(NAME)

