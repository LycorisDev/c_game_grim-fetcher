CC = cc
CFLAGS = -Wall -Wextra -Werror
BIN = grim_fetcher

HDR = grim_fetcher.h
SRC = $(wildcard src/*.c) $(wildcard src/*/*.c)
OBJ = $(SRC:.c=.o)

MLX_DIR = .mlx
MLX_LIB = $(MLX_DIR)/libmlx.a
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11

all: $(MLX_LIB) $(BIN)

$(MLX_LIB):
	@make -s -C $(MLX_DIR)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(MLX_FLAGS)

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -I. -o $@ -c $<

.PHONY: clean fclean re

clean:
	rm -f $(OBJ)

fclean: clean
	rm -rf $(BIN)

re: fclean all
