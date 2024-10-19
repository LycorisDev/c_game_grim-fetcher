#ifndef GRIM_FETCHER_H
# define GRIM_FETCHER_H

# include ".mlx/mlx.h"
# include ".mlx/mlx_int.h"
# include <stdio.h>
# include <stdlib.h>
# include <stdarg.h>
# include <unistd.h>
# include <fcntl.h>

# define RES_WIDTH  640
# define RES_HEIGHT 360
# define SPRITE_LEN	63

# define BTN_CLICK_LEFT  1
# define BTN_CLICK_WHEEL 2
# define BTN_CLICK_RIGHT 3
# define BTN_SCROLL_UP   4
# define BTN_SCROLL_DOWN 5
# define KEY_W 119
# define KEY_A 97
# define KEY_S 115
# define KEY_D 100
# define KEY_ENTER  65293
# define KEY_ESCAPE 65307
# define KEY_LEFT   65361
# define KEY_UP     65362
# define KEY_RIGHT  65363
# define KEY_DOWN   65364
# define KEY_F11    65480

typedef unsigned char	t_ubyte;
typedef unsigned int	t_uint;

typedef struct s_ivec2
{
	int	x;
	int	y;
}	t_ivec2;

typedef struct s_vec2
{
	double	x;
	double	y;
}	t_vec2;

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_spr
{
	char	*id;
	t_ivec2	size;
	t_ivec2	shadow_offset;
	int		still_frame;
	int		cycle_len;
	int		cycle_index;
	long	elapsed_time_in_ms;
	long	cycle_time_in_ms;
	t_uint	**cycle;
	t_uint	**cycle_shadow;
}	t_spr;

typedef struct s_player
{
	t_ivec2	pos;
	int		steps;
	int		collected;
	int		to_collect;
	int		health;
	int		stamina;
}	t_player;

typedef struct s_cell	t_cell;

struct s_cell
{
	char	symbol;
	t_spr	*spr;
	int		is_obstacle;
	int		is_visited;
	t_ivec2	pos;
	t_cell	*neighbors[4];
	t_cell	*parent;
	double	local_goal;
	double	global_goal;
};

typedef struct s_map
{
	char	*name;
	t_ivec2	size;
	t_cell	*cells;
}	t_map;

typedef struct s_frame
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_length;
	int		endian;
	int		thickness;
	t_ivec2	size;
}	t_frame;

typedef struct s_win
{
	void		*mlx;
	void		*win;
	t_ivec2		size;
	t_ivec2		cursor;
	int			click;
	t_ivec2		click_pos;
	int			zoom;
	t_frame		frame[3];
	int			curr_frame;
	t_spr		sprites[SPRITE_LEN];
	t_map		map;
	t_player	p;
	int			game_over;
	t_list		*clicked_path;
}	t_win;

typedef struct s_xpm_seg
{
	char	*id;
	t_ivec2	size;
	t_ivec2	shadow_offset;
	int		still_frame;
	int		cycle_len;
	long	cycle_time_in_ms;
	t_ivec2	*cycle;
}	t_xpm_seg;

typedef struct s_xpm
{
	char		*path;
	char		*path_shadow;
	t_ivec2		size;
	t_uint		*data;
	t_uint		*data_shadow;
	int			segment_len;
	t_xpm_seg	*seg;
}	t_xpm;

/* Init --------------------------------------------------------------------- */

int		set_sprite_array(t_win *win, char *path);
int		set_map_and_player(t_win *win, int argc, char *path);
int		create_window(t_win *win, int width, int height);
void	add_outline_to_font(t_spr *font);

/* Deinit ------------------------------------------------------------------- */

void	free_and_reset_xpm_data(t_xpm *file);
void	free_sprites(t_win *win);
void	free_ber_data(char **ber);
void	release_resources(t_win *win);

/* Events ------------------------------------------------------------------- */

int		on_close(t_win *win);
int		on_mouse_move(t_win *win);
int		on_mouse_button(int button, int x, int y, t_win *win);
int		on_key_press(int keycode, t_win *win);

/* Colors ------------------------------------------------------------------- */

t_ubyte	get_alpha(t_uint color);
t_ubyte	get_red(t_uint color);
t_ubyte	get_green(t_uint color);
t_ubyte	get_blue(t_uint color);
void	set_alpha(t_uint *color, t_ubyte value);
void	set_red(t_uint *color, t_ubyte value);
void	set_green(t_uint *color, t_ubyte value);
void	set_blue(t_uint *color, t_ubyte value);

t_uint	get_color(t_ubyte alpha, t_ubyte red, t_ubyte green, t_ubyte blue);
t_uint	get_color_from_hex(char *str);
t_uint	get_alpha_blended_color(t_ubyte prev[4], t_ubyte curr[4]);

/* Draw --------------------------------------------------------------------- */

int		render_loop(t_win *win);
long	get_delta_time(void);
t_spr	*get_sprite(t_win *win, char *id);
t_spr	*get_spr_by_symbol(t_win *win, char symbol);
void	set_vec2(t_vec2 *vector, double x, double y);
void	set_ivec2(t_ivec2 *vector, int x, int y);
void	render_background(t_win *win, t_frame *frame);
void	draw_ground(t_win *win, t_frame *frame, t_ivec2 pos);
void	draw_walls(t_win *win, t_frame *frame, t_ivec2 pos, t_ivec2 i);
void	copy_frame(t_frame *dst, t_frame *src, int zoom_factor);
void	render_gui(t_win *win, t_frame *frame, long dt);

void	put_pixel(t_frame *frame, t_uint color, int x, int y);
void	draw_point(t_frame *frame, t_uint color, t_ivec2 p);
void	draw_line(t_frame *frame, t_uint color, t_ivec2 p, t_ivec2 dir);
void	draw_sprite(t_frame *frame, t_spr *sprite, t_ivec2 pos, long dt_ms);
void	draw_font_default(t_win *win, t_frame *frame, t_ivec2 *pos, char *str);

/* Game --------------------------------------------------------------------- */

t_list	*get_path(t_win *win, t_cell *start, t_cell *end);
void	move_along_clicked_path(t_win *win, long dt);
t_cell	*find_cell_by_symbol(t_cell *map, char symbol);
void	move_player(t_win *win, t_ivec2 move);
void	move_symbol(t_map *map, t_ivec2 start, t_ivec2 end);
void	move_slimes(t_win *win, long dt);
t_ivec2	slime_get_move_to_kill_player(t_cell *slime);
void	game_over_screen(t_win *win, int victory);

/* Math --------------------------------------------------------------------- */

double	ft_abs(double number);
double	ft_clamp(double number, double min, double max);
double	ft_pow(double base, double exp);
double	ft_sqrt(double number);
int		rng_minmax(int *seed, int min, int max);
double	get_dist_euclidean(double ax, double ay, double bx, double by);
double	get_dist_manhattan(double ax, double ay, double bx, double by);

/* Libft -------------------------------------------------------------------- */

int		ft_isspace(int c);
int		ft_isdigit(int c);
int		ft_isupper(int c);
int		ft_islower(int c);
size_t	ft_strlen(char *s);
char	*ft_strchr(char *s, int c, int reverse);
size_t	ft_strlcat(char *dest, char *src, size_t size);
char	*ft_strdup(char *s);
void	ft_bzero(void *s, size_t n);
int		ft_atoi(char *n);
int		ft_strcmp(char *s1, char *s2);
int		ft_strncmp(char *s1, char *s2, size_t n);
char	*ft_itoa(int number);
char	*get_next_line(int fd);
void	ft_dprintf(int fd, char *format, ...);
t_list	*lst_new(void *content);
void	lst_add_front(t_list **lst, t_list *new);
int		lst_add_sorted(t_list **lst, t_list *new, int (*f)(void *, void *));
void	lst_pop(t_list **lst, void (*del)(void *));
void	lst_clear(t_list **lst, void (*del)(void *));
int		lst_size(t_list *lst);

#endif
