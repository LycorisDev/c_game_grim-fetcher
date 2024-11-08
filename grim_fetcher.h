#ifndef GRIM_FETCHER_H
#define GRIM_FETCHER_H

#include ".mlx/mlx.h"
#include ".mlx/mlx_int.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>

#define RES_WIDTH  640
#define RES_HEIGHT 360
#define SPRITE_LEN 63

#define BTN_CLICK_LEFT  1
#define BTN_CLICK_WHEEL 2
#define BTN_CLICK_RIGHT 3
#define BTN_SCROLL_UP   4
#define BTN_SCROLL_DOWN 5
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_ENTER  65293
#define KEY_ESCAPE 65307
#define KEY_LEFT   65361
#define KEY_UP     65362
#define KEY_RIGHT  65363
#define KEY_DOWN   65364
#define KEY_F11    65480

typedef unsigned char t_ubyte;
typedef unsigned int t_uint;

typedef struct s_ivec2
{
    int x;
    int y;
} t_ivec2;

typedef struct s_vec2
{
    double x;
    double y;
} t_vec2;

typedef struct s_list
{
    void          *content;
    struct s_list *next;
} t_list;

typedef struct s_spr
{
    char    *id;
    t_ivec2 size;
    t_ivec2 shadow_offset;
    int     still_frame;
    int     cycle_len;
    int     cycle_index;
    long    elapsed_time_in_ms;
    long    cycle_time_in_ms;
    t_uint  **cycle;
    t_uint  **cycle_shadow;
} t_spr;

typedef struct s_player
{
    t_ivec2 pos;
    int     steps;
    int     collected;
    int     to_collect;
    int     health;
    int     stamina;
} t_player;

typedef struct s_cell t_cell;

struct s_cell
{
    char    symbol;
    t_spr   *spr;
    int     is_obstacle;
    int     is_visited;
    t_ivec2 pos;
    t_cell  *neighbors[4];
    t_cell  *parent;
    double  local_goal;
    double  global_goal;
};

typedef struct s_map
{
    char    *name;
    t_ivec2 size;
    t_cell  *cells;
} t_map;

typedef struct s_frame
{
    void    *img;
    char    *addr;
    int     bpp;
    int     line_length;
    int     endian;
    int     thickness;
    t_ivec2 size;
} t_frame;

typedef struct s_win
{
    void     *mlx;
    void     *win;
    t_ivec2  size;
    t_ivec2  cursor;
    int      click;
    t_ivec2  click_pos;
    int      zoom;
    t_frame  frame[3];
    int      curr_frame;
    t_spr    sprites[SPRITE_LEN];
    t_map    map;
    t_player p;
    int      game_over;
    t_list   *clicked_path;
} t_win;

typedef struct s_xpm_seg
{
    char    *id;
    t_ivec2 size;
    t_ivec2 shadow_offset;
    int     still_frame;
    int     cycle_len;
    long    cycle_time_in_ms;
    t_ivec2 *cycle;
} t_xpm_seg;

typedef struct s_xpm
{
    char      *path;
    char      *path_shadow;
    t_ivec2   size;
    t_uint    *data;
    t_uint    *data_shadow;
    int       segment_len;
    t_xpm_seg *seg;
} t_xpm;

/* Init --------------------------------------------------------------------- */

int     set_sprite_array(t_win *win, char *path);
int     set_map_and_player(t_win *win, int argc, char *path);
int     create_window(t_win *win, int width, int height);
void    add_outline_to_font(t_spr *font);

/* Deinit ------------------------------------------------------------------- */

void    free_and_reset_xpm_data(t_xpm *file);
void    free_sprites(t_win *win);
void    free_ber_data(char **ber);
void    release_resources(t_win *win);

/* Events ------------------------------------------------------------------- */

int     on_close(t_win *win);
int     on_mouse_move(t_win *win);
int     on_mouse_button(int button, int x, int y, t_win *win);
int     on_key_press(int keycode, t_win *win);

/* Colors ------------------------------------------------------------------- */

t_uint  get_color(t_ubyte alpha, t_ubyte red, t_ubyte green, t_ubyte blue);
t_uint  get_color_from_hex(char *str);
t_uint  get_alpha_blended_color(t_ubyte prev[4], t_ubyte curr[4]);
t_ubyte get_alpha(t_uint color);
t_ubyte get_red(t_uint color);
t_ubyte get_green(t_uint color);
t_ubyte get_blue(t_uint color);
void    set_alpha(t_uint *color, t_ubyte value);
void    set_red(t_uint *color, t_ubyte value);
void    set_green(t_uint *color, t_ubyte value);
void    set_blue(t_uint *color, t_ubyte value);

/* Draw --------------------------------------------------------------------- */

int     render_loop(t_win *win);
long    get_delta_time(void);
t_spr   *get_sprite(t_win *win, char *id);
t_spr   *get_spr_by_symbol(t_win *win, char symbol);
void    render_background(t_win *win, t_frame *frame);
void    draw_walls(t_win *win, t_frame *frame, t_ivec2 pos, t_ivec2 i);
void    copy_frame(t_frame *dst, t_frame *src, int zoom_factor);
void    render_gui(t_win *win, t_frame *frame, long dt);

void    put_pixel(t_frame *frame, t_uint color, int x, int y);
void    draw_point(t_frame *frame, t_uint color, t_ivec2 p);
void    draw_line(t_frame *frame, t_uint color, t_ivec2 p, t_ivec2 dir);
void    draw_sprite(t_frame *frame, t_spr *sprite, t_ivec2 pos, long dt_ms);
void    draw_font_default(t_win *win, t_frame *frame, t_ivec2 *pos, char *str);

/* Game --------------------------------------------------------------------- */

t_list  *get_path(t_win *win, t_cell *start, t_cell *end);
void    move_along_clicked_path(t_win *win, long dt);
t_cell  *find_cell_by_symbol(t_cell *map, char symbol);
void    move_player(t_win *win, t_ivec2 move);
void    move_symbol(t_map *map, t_ivec2 start, t_ivec2 end);
void    move_slimes(t_win *win, long dt);
t_ivec2 slime_get_move_to_kill_player(t_cell *slime);

/* Math --------------------------------------------------------------------- */

double  f_abs(double number);
double  f_clamp(double number, double min, double max);
double  f_pow(double base, double exp);
double  f_sqrt(double number);
double  f_dist_euclidean(double ax, double ay, double bx, double by);
double  f_dist_manhattan(double ax, double ay, double bx, double by);
int     i_abs(int number);
int     i_max(int a, int b);
int     rng_minmax(int *seed, int min, int max);
void    set_vec2(t_vec2 *vector, double x, double y);
void    set_ivec2(t_ivec2 *vector, int x, int y);

/* Utils -------------------------------------------------------------------- */

char    *gnl(int fd);
char    *itoa_dec(int number);
t_list  *list_new(void *content);
void    list_add_front(t_list **list, t_list *new);
int     list_add_sorted(t_list **list, t_list *new, int (*f)(void *, void *));
void    list_pop(t_list **list, void (*del)(void *));
void    list_clear(t_list **list, void (*del)(void *));
int     list_size(t_list *list);

/* Misc --------------------------------------------------------------------- */

int     get_ber_fd(int argc, char *path);
char    **copy_ber(int *fd, char *path);
char    **get_ber_data(int argc, char *path);
t_spr   *get_spr_by_symbol(t_win *win, char symbol);
int     are_collectibles_and_exit_accessible(t_win *win);
int     create_background(t_win *win);
int     set_xpm_file_obj(t_xpm *file, int fd);
int     get_color_bpp(t_img *img, size_t i, int is_shadow);
int     create_sprites_from_file(t_win *win, t_xpm *file, int *i_spr);

#endif
