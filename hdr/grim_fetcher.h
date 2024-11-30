#ifndef GRIM_FETCHER_H
#define GRIM_FETCHER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include "gl_functions.h"

#define RES_WIDTH  640
#define RES_HEIGHT 360
#define SPRITE_LEN 63

typedef unsigned char t_ubyte;
typedef unsigned int t_uint;

typedef struct s_uivec2
{
    t_uint x;
    t_uint y;
} t_uivec2;

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

typedef struct s_color
{
    GLubyte r;
    GLubyte g;
    GLubyte b;
    GLubyte a;
} t_color;

typedef struct s_vert
{
    t_ivec2 coord;
    t_color color;
} t_vert;

typedef struct s_list
{
    void          *data;
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
    t_color **cycle;
    t_color **cycle_shadow;
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

typedef struct s_img_seg
{
    char    *id;
    t_ivec2 size;
    t_ivec2 shadow_offset;
    int     still_frame;
    int     cycle_len;
    long    cycle_time_in_ms;
    t_ivec2 *cycle;
} t_img_seg;

typedef struct s_img
{
    char      *path;
    char      *path_shadow;
    t_ivec2   size;
    t_color   *data;
    t_color   *data_shadow;
    int       segment_len;
    t_img_seg *seg;
} t_img;

typedef struct s_png
{
    t_ivec2  size;
    t_ubyte  *buf;
} t_png;

typedef struct s_frame
{
    GLuint   id;
    t_ivec2  size;
    t_ivec2  real_size;
    int      thickness;
    GLubyte  *buf;
} t_frame;

typedef struct s_res
{
    t_ivec2 monitor_size;
    double aspect_ratio;
    t_ivec2 window_size_default;
    t_ivec2 window_size;
    t_ivec2 window_position;
    t_ivec2 viewport_size;
    t_ivec2 viewport_offset;
    t_ivec2 fullscreen;
} t_res;

#define NBR_FRAMES 3
typedef struct s_man
{
    GLFWwindow *window;
    GLuint     shader_program;
    double     delta_time;
    double     fps_count;
    t_res      res;
    t_frame    *frame[NBR_FRAMES + 1];
    int        curr_frame;
    t_player   player;
    t_ivec2    cursor;
    int        click;
    t_ivec2    click_pos;
    int        zoom;
    t_spr      sprites[SPRITE_LEN];
    t_map      map;
    int        game_over;
    t_list     *clicked_path;
} t_man;

extern t_man man;

/* Init --------------------------------------------------------------------- */

int        set_sprite_array(char *path);
int        set_map_and_player(int argc, char *path);
void       add_outline_to_font(t_spr *font);
char       *read_file(char* filepath);
int        get_map_fd(int argc, char *path);
char       **copy_map(int *fd, char *path);
char       **get_map_data(int argc, char *path);
void       free_map_data(char **map);
t_spr      *get_spr_by_symbol(char symbol);
int        are_collectibles_and_exit_accessible(void);
t_png      *load_image_from_file(char* png_path);
void       free_image(t_png* img);
int        create_background(void);
int        set_img_file_obj(t_img *file, int fd);
int        create_sprites_from_file(t_img *file, int *i_spr);
void       free_sprites(void);
int        create_mesh(void);
void       render_mesh(void);
void       free_mesh(void);
int        create_shader_program(void);
void       free_shader_program(void);
int        create_uniform(void);
void       free_uniform(void);
int        create_frames(void);
void       free_frames(void);
GLFWwindow *get_window(char* title);
void       toggle_fullscreen(GLFWwindow* window);
void       physical_key_callback(GLFWwindow* window, int key, int scancode,
               int action, int mods);
void       scroll_callback(GLFWwindow* window, double x_offset,
               double y_offset);
void       mouse_callback(GLFWwindow* window, int button, int action, int mods);
void       cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

/* Colors ------------------------------------------------------------------- */

t_color    get_color_rgba(GLubyte r, GLubyte g, GLubyte b, GLubyte a);
t_color    get_color_hex(char* str, GLubyte alpha);
t_color    get_alpha_blended_color(t_color prev, t_color new);
t_color    get_frame_color(t_frame* f, int x, int y);
int        cmp_color(t_color a, t_color b);

/* Draw --------------------------------------------------------------------- */

void       use_frame(t_frame* f);
void       clear_drawing(t_frame *f, GLubyte alpha);
void       save_drawing(t_frame* f);
int        render(void);
long       get_delta_time(void);
t_spr      *get_sprite(char *id);
t_spr      *get_spr_by_symbol(char symbol);
void       render_background(t_frame *frame);
void       draw_walls(t_frame *frame, t_ivec2 pos, t_ivec2 i);
void       copy_frame(t_frame *dst, t_frame *src, int zoom_factor);
void       render_gui(t_frame *frame, long dt);
void       draw_point(t_frame *f, t_color c, int x, int y);
void       draw_sprite(t_frame *frame, t_spr *sprite, t_ivec2 pos, long dt_ms);
void       draw_cursor(t_frame *frame, t_spr *sprite, t_ivec2 p, int cyc);
void       draw_font_default(t_frame *frame, t_ivec2 *pos, char *str);

/* Game --------------------------------------------------------------------- */

t_list     *get_path(t_cell *start, t_cell *end);
void       move_along_clicked_path(long dt);
t_cell     *find_cell_by_symbol(t_cell *map, char symbol);
void       move_player(int x, int y);
void       move_symbol(t_map *map, t_ivec2 start, t_ivec2 end);
void       move_slimes(long dt);

/* Math --------------------------------------------------------------------- */

double     f_abs(double number);
double     f_clamp(double number, double min, double max);
double     f_floor(double number);
double     f_ceil(double number);
double     f_round(double number);
double     f_pow(double base, double exp);
double     f_sqrt(double number);
double     f_dist_euclidean(double ax, double ay, double bx, double by);
double     f_dist_manhattan(double ax, double ay, double bx, double by);
int        i_abs(int number);
int        i_min(int a, int b);
int        i_max(int a, int b);
int        i_clamp(int number, int min, int max);
int        rng_minmax(int *seed, int min, int max);
void       set_vec2(t_vec2 *vector, double x, double y);
void       set_ivec2(t_ivec2 *vector, int x, int y);

/* Utils -------------------------------------------------------------------- */

char       *gnl(int fd);
char       *itoa_dec(int number);
t_list     *list_new(void *content);
void       list_add_front(t_list **list, t_list *new);
int        list_add_sorted(t_list **list, t_list *new,
               int (*f)(void *, void *));
void       list_pop(t_list **list, void (*del)(void *));
void       list_clear(t_list **list, void (*del)(void *));
int        list_size(t_list *list);

#endif
