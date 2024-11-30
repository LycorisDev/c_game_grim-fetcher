#include "grim_fetcher.h"

static void    game_over_screen(int victory);
static void    render_map(t_frame *frame, long dt);
static void    set_ivec2_vals(t_ivec2 *cs, t_ivec2 *op, t_ivec2 *p);
static t_ivec2 get_elem_pos(t_ivec2 i, t_ivec2 pos);
static void    draw_ground(t_frame *frame, t_ivec2 pos);

int render_loop(void)
{
    long    dt;
    t_frame *frame;
    t_frame *tmp;

    if (man.game_over)
    {
        game_over_screen(man.game_over == 1);
        return 0;
    }
    dt = get_delta_time();
    move_slimes(dt);
    move_along_clicked_path(dt);
    frame = man.frame[man.curr_frame];
    tmp = man.frame[2];
    clear_drawing(tmp, 0);
    clear_drawing(frame, 0);
    render_background(frame);
    render_map(tmp, dt);
    copy_frame(frame, tmp, man.zoom);
    render_gui(frame, dt);
    return 0;
}

t_spr *get_spr_by_symbol(char symbol)
{
    if (symbol == 'E')
        return &man.sprites[7];
    else if (symbol == 'e')
        return &man.sprites[8];
    else if (symbol == 'C')
        return &man.sprites[10];
    else if (symbol == 'P')
        return &man.sprites[13];
    else if (symbol == 'B')
        return &man.sprites[36];
    else if (symbol == 'G')
        return &man.sprites[49];
    return 0;
}

static void game_over_screen(int victory)
{
    long    dt;
    t_ivec2 pos;
    t_frame *frame;

    dt = get_delta_time();
    frame = man.frame[man.curr_frame];
    render_background(frame);
    render_gui(frame, dt);
    set_ivec2(&pos, RES_WIDTH / 2, RES_HEIGHT / 2);
    if (victory)
    {
        pos.x -= strlen("You won! :D") / 2 * 8;
        draw_font_default(frame, &pos, "You won! :D");
    }
    else
    {
        pos.x -= strlen("You got killed by a slime :(") / 2 * 7;
        draw_font_default(frame, &pos, "You got killed by a slime :(");
    }
    return;
}

static void set_ivec2_vals(t_ivec2 *cs, t_ivec2 *op, t_ivec2 *p)
{
    t_cell  *cell;
    t_ivec2 zoom_offset;

    set_ivec2(cs, man.sprites[9].size.x * 3, man.sprites[9].size.y * 3);
    cell = find_cell_by_symbol(man.map.cells, 'P');
    set_ivec2(&zoom_offset, 13 / man.zoom, 7 / man.zoom);
    op->x = (zoom_offset.x - cell->pos.x) * cs->x;
    op->y = (zoom_offset.y - cell->pos.y) * cs->y;
    set_ivec2(p, op->x, op->y);
    return;
}

static t_ivec2 get_elem_pos(t_ivec2 i, t_ivec2 pos)
{
    t_ivec2 p;

    set_ivec2(&p, pos.x - man.sprites[9].size.x / 2,
        pos.y - man.sprites[9].size.y / 2);
    if (man.map.cells[i.y * man.map.size.x + i.x].symbol == 'E'
        || man.map.cells[i.y * man.map.size.x + i.x].symbol == 'e')
        set_ivec2(&p, pos.x, pos.y);
    return p;
}

static void render_map(t_frame *frame, long dt)
{
    t_ivec2 i;
    t_ivec2 cell_size;
    t_ivec2 origin_pos;
    t_ivec2 pos;

    set_ivec2_vals(&cell_size, &origin_pos, &pos);
    i.y = -1;
    while (++i.y < man.map.size.y)
    {
        i.x = -1;
        while (++i.x < man.map.size.x)
        {
            if (man.map.cells[i.y * man.map.size.x + i.x].symbol != '1')
                draw_ground(frame, pos);
            if (man.map.cells[i.y * man.map.size.x + i.x].spr)
                draw_sprite(frame,
                    man.map.cells[i.y * man.map.size.x + i.x].spr,
                    get_elem_pos(i, pos), dt);
            else if (man.map.cells[i.y * man.map.size.x + i.x].symbol == '1')
                draw_walls(frame, pos, i);
            pos.x += cell_size.x;
        }
        set_ivec2(&pos, origin_pos.x, pos.y + cell_size.y);
    }
    return;
}

static void draw_ground(t_frame *frame, t_ivec2 pos)
{
    draw_sprite(frame, &man.sprites[9], pos, 0);
    set_ivec2(&pos, pos.x + man.sprites[9].size.x, pos.y);
    draw_sprite(frame, &man.sprites[9], pos, 0);
    set_ivec2(&pos, pos.x + man.sprites[9].size.x, pos.y);
    draw_sprite(frame, &man.sprites[9], pos, 0);
    set_ivec2(&pos, pos.x - man.sprites[9].size.x * 2,
        pos.y + man.sprites[9].size.y);
    draw_sprite(frame, &man.sprites[9], pos, 0);
    set_ivec2(&pos, pos.x + man.sprites[9].size.x, pos.y);
    draw_sprite(frame, &man.sprites[9], pos, 0);
    set_ivec2(&pos, pos.x + man.sprites[9].size.x, pos.y);
    draw_sprite(frame, &man.sprites[9], pos, 0);
    set_ivec2(&pos, pos.x - man.sprites[9].size.x * 2,
        pos.y + man.sprites[9].size.y);
    draw_sprite(frame, &man.sprites[9], pos, 0);
    set_ivec2(&pos, pos.x + man.sprites[9].size.x, pos.y);
    draw_sprite(frame, &man.sprites[9], pos, 0);
    set_ivec2(&pos, pos.x + man.sprites[9].size.x, pos.y);
    draw_sprite(frame, &man.sprites[9], pos, 0);
    return;
}
