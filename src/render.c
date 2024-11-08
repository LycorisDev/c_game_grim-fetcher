#include "grim_fetcher.h"

static void    game_over_screen(t_win *win, int victory);
static void    render_map(t_win *win, t_frame *frame, long dt);
static void    set_ivec2_vals(t_win *w, t_ivec2 *cs, t_ivec2 *op, t_ivec2 *p);
static t_ivec2 get_elem_pos(t_win *win, t_ivec2 i, t_ivec2 pos);
static void    draw_ground(t_win *win, t_frame *frame, t_ivec2 pos);

int render_loop(t_win *win)
{
    long dt;
    t_frame *frame;
    t_frame *tmp;

    if (win->game_over)
    {
        game_over_screen(win, win->game_over == 1);
        return 0;
    }
    dt = get_delta_time();
    move_slimes(win, dt);
    move_along_clicked_path(win, dt);
    frame = &win->frame[win->curr_frame];
    tmp = &win->frame[2];
    bzero(tmp->addr, tmp->size.x * tmp->size.y * (tmp->bpp / 8));
    bzero(frame->addr, frame->size.x * frame->size.y * (frame->bpp / 8));
    render_background(win, frame);
    render_map(win, tmp, dt);
    copy_frame(frame, tmp, win->zoom);
    render_gui(win, frame, dt);
    on_mouse_move(win);
    win->curr_frame = (win->curr_frame + 1) % 2;
    mlx_put_image_to_window(win->mlx, win->win, frame->img, 0, 0);
    return 0;
}

t_spr *get_spr_by_symbol(t_win *win, char symbol)
{
    if (symbol == 'E')
        return &win->sprites[7];
    else if (symbol == 'e')
        return &win->sprites[8];
    else if (symbol == 'C')
        return &win->sprites[10];
    else if (symbol == 'P')
        return &win->sprites[13];
    else if (symbol == 'B')
        return &win->sprites[36];
    else if (symbol == 'G')
        return &win->sprites[49];
    return 0;
}

static void game_over_screen(t_win *win, int victory)
{
    long    dt;
    t_ivec2 pos;
    t_frame *frame;

    dt = get_delta_time();
    frame = &win->frame[win->curr_frame];
    render_background(win, frame);
    render_gui(win, frame, dt);
    set_ivec2(&pos, RES_WIDTH / 2, RES_HEIGHT / 2);
    if (victory)
    {
        pos.x -= strlen("You won! :D") / 2 * 8;
        draw_font_default(win, frame, &pos, "You won! :D");
    }
    else
    {
        pos.x -= strlen("You got killed by a slime :(") / 2 * 7;
        draw_font_default(win, frame, &pos, "You got killed by a slime :(");
    }
    on_mouse_move(win);
    win->curr_frame = (win->curr_frame + 1) % 2;
    mlx_put_image_to_window(win->mlx, win->win, frame->img, 0, 0);
    return;
}

static void set_ivec2_vals(t_win *w, t_ivec2 *cs, t_ivec2 *op, t_ivec2 *p)
{
    t_cell  *cell;
    t_ivec2 zoom_offset;

    set_ivec2(cs, w->sprites[9].size.x * 3, w->sprites[9].size.y * 3);
    cell = find_cell_by_symbol(w->map.cells, 'P');
    set_ivec2(&zoom_offset, 13 / w->zoom, 7 / w->zoom);
    op->x = (zoom_offset.x - cell->pos.x) * cs->x;
    op->y = (zoom_offset.y - cell->pos.y) * cs->y;
    set_ivec2(p, op->x, op->y);
    return;
}

static t_ivec2 get_elem_pos(t_win *win, t_ivec2 i, t_ivec2 pos)
{
    t_ivec2 p;

    set_ivec2(&p, pos.x - win->sprites[9].size.x / 2,
        pos.y - win->sprites[9].size.y / 2);
    if (win->map.cells[i.y * win->map.size.x + i.x].symbol == 'E'
        || win->map.cells[i.y * win->map.size.x + i.x].symbol == 'e')
        set_ivec2(&p, pos.x, pos.y);
    return p;
}

static void render_map(t_win *win, t_frame *frame, long dt)
{
    t_ivec2 i;
    t_ivec2 cell_size;
    t_ivec2 origin_pos;
    t_ivec2 pos;

    set_ivec2_vals(win, &cell_size, &origin_pos, &pos);
    i.y = -1;
    while (++i.y < win->map.size.y)
    {
        i.x = -1;
        while (++i.x < win->map.size.x)
        {
            if (win->map.cells[i.y * win->map.size.x + i.x].symbol != '1')
                draw_ground(win, frame, pos);
            if (win->map.cells[i.y * win->map.size.x + i.x].spr)
                draw_sprite(frame,
                    win->map.cells[i.y * win->map.size.x + i.x].spr,
                    get_elem_pos(win, i, pos), dt);
            else if (win->map.cells[i.y * win->map.size.x + i.x].symbol == '1')
                draw_walls(win, frame, pos, i);
            pos.x += cell_size.x;
        }
        set_ivec2(&pos, origin_pos.x, pos.y + cell_size.y);
    }
    return;
}

static void draw_ground(t_win *win, t_frame *frame, t_ivec2 pos)
{
    draw_sprite(frame, &win->sprites[9], pos, 0);
    set_ivec2(&pos, pos.x + win->sprites[9].size.x, pos.y);
    draw_sprite(frame, &win->sprites[9], pos, 0);
    set_ivec2(&pos, pos.x + win->sprites[9].size.x, pos.y);
    draw_sprite(frame, &win->sprites[9], pos, 0);
    set_ivec2(&pos, pos.x - win->sprites[9].size.x * 2,
        pos.y + win->sprites[9].size.y);
    draw_sprite(frame, &win->sprites[9], pos, 0);
    set_ivec2(&pos, pos.x + win->sprites[9].size.x, pos.y);
    draw_sprite(frame, &win->sprites[9], pos, 0);
    set_ivec2(&pos, pos.x + win->sprites[9].size.x, pos.y);
    draw_sprite(frame, &win->sprites[9], pos, 0);
    set_ivec2(&pos, pos.x - win->sprites[9].size.x * 2,
        pos.y + win->sprites[9].size.y);
    draw_sprite(frame, &win->sprites[9], pos, 0);
    set_ivec2(&pos, pos.x + win->sprites[9].size.x, pos.y);
    draw_sprite(frame, &win->sprites[9], pos, 0);
    set_ivec2(&pos, pos.x + win->sprites[9].size.x, pos.y);
    draw_sprite(frame, &win->sprites[9], pos, 0);
    return;
}
