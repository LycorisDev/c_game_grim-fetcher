CC = gcc
CFLAGS = -Iinclude -Wall -Wextra
LDFLAGS = -lGL -Llib -lglfw34 -lm
BIN = grim_fetcher

SRC = $(wildcard lib/*.c) $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJ = $(SRC:.c=.o)

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: all clean fclean re

clean:
	rm -f $(OBJ)

fclean: clean
	rm -rf $(BIN)

re: fclean all
