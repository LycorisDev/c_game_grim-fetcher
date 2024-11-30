CC = cc
CFLAGS = -Wall -Wextra
LDFLAGS = -lm -lGL -lglfw
BIN = grim_fetcher

HDR_DIR = hdr
HDR = $(HDR_DIR)/grim_fetcher.h
SRC = $(wildcard lib/*.c) $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJ = $(SRC:.c=.o)

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -I$(HDR_DIR) -o $@ -c $<

.PHONY: clean fclean re

clean:
	rm -f $(OBJ)

fclean: clean
	rm -rf $(BIN)

re: fclean all
