CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = grim_fetcher

HDR = grim_fetcher.h
SRC = background.c ber_copy.c ber_data.c clicked_move.c color_ar.c color.c \
color_gb.c cursor.c dist.c draw_font.c draw_line.c draw_point.c draw_sprite.c \
end_game.c events.c font_init.c free.c gui.c libft_0.c libft_1.c libft_2.c \
libft_3.c libft_4.c libft_5.c libft_6.c main.c map_access.c math.c misc.c \
movement.c pathfinding.c player.c render.c render_ground.c render_walls.c \
set_map.c set_player.c set_sprites.c time.c xpm_color.c xpm_file.c xpm_segment.c
OBJ = $(SRC:.c=.o)

MLX_DIR = .mlx
MLX_LIB = $(MLX_DIR)/libmlx.a
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11

all: mlx $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(MLX_FLAGS)

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -I. -o $@ -c $<

$(MLX_LIB):
	@git clone -q https://github.com/42Paris/minilibx-linux.git
	@mv minilibx-linux $(MLX_DIR)
	@echo "Downloaded Linux version of MLX library: $(MLX_DIR)"
	@make -s -C $(MLX_DIR)

.PHONY: mlx bonus clean fclean re

mlx: $(MLX_LIB)

bonus:
	@if ! nm $(NAME) | grep -q "get_time" > /dev/null; then \
		$(MAKE) fclean; \
	fi
	$(eval CFLAGS := $(CFLAGS) -D BONUS)
	$(MAKE) CFLAGS="$(CFLAGS)"

clean:
	rm -f $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
