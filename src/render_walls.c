#include "grim_fetcher.h"

static void draw_top_walls(t_frame *f, t_ivec2 p, t_ivec2 i);
static void draw_top_walls_bis(t_frame *f, t_ivec2 pp, t_ivec2 i);
static void draw_lat_walls(t_frame *f, t_ivec2 p, t_ivec2 i);
static void draw_bot_walls(t_frame *f, t_ivec2 p, t_ivec2 i);

void draw_walls(t_frame *frame, t_ivec2 pos, t_ivec2 i)
{
    draw_top_walls(frame, pos, i);
    draw_top_walls_bis(frame, pos, i);
    draw_lat_walls(frame, pos, i);
    draw_bot_walls(frame, pos, i);
    return;
}

static void draw_top_walls(t_frame *f, t_ivec2 p, t_ivec2 i)
{
    t_ivec2 pd;

    if (i.y > 0 && man.map.cells[(i.y - 1) * man.map.size.x + i.x].symbol != '1')
    {
        set_ivec2(&pd, p.x, p.y + man.sprites[5].size.y);
        draw_sprite(f, &man.sprites[5], pd, 0);
        set_ivec2(&pd, pd.x + man.sprites[5].size.x, pd.y);
        draw_sprite(f, &man.sprites[5], pd, 0);
        set_ivec2(&pd, pd.x + man.sprites[5].size.x, pd.y);
        draw_sprite(f, &man.sprites[5], pd, 0);
        draw_sprite(f, &man.sprites[4], p, 0);
    }
    if (i.y > 0 && man.map.cells[(i.y - 1) * man.map.size.x + i.x].symbol == '1'
        && i.x > 0
        && man.map.cells[i.y * man.map.size.x + (i.x - 1)].symbol == '1'
        && man.map.cells[(i.y - 1) * man.map.size.x + (i.x - 1)].symbol != '1')
    {
        set_ivec2(&pd, p.x, p.y + man.sprites[5].size.y);
        draw_sprite(f, &man.sprites[5], pd, 0);
        draw_sprite(f, &man.sprites[3], p, 0);
    }
    return;
}

static void draw_top_walls_bis(t_frame *f, t_ivec2 p, t_ivec2 i)
{
    t_ivec2 pd;
    t_ivec2 pp;

    set_ivec2(&pp, p.x + man.sprites[3].size.x * 2, p.y);
    if (i.y > 0 && man.map.cells[(i.y - 1) * man.map.size.x + i.x].symbol == '1'
        && i.x < man.map.size.x - 1
        && man.map.cells[i.y * man.map.size.x + (i.x + 1)].symbol == '1'
        && man.map.cells[(i.y - 1) * man.map.size.x + (i.x + 1)].symbol != '1')
    {
        set_ivec2(&pd, p.x + man.sprites[5].size.x * 2,
            p.y + man.sprites[5].size.y);
        draw_sprite(f, &man.sprites[5], pd, 0);
        draw_sprite(f, &man.sprites[3], pp, 0);
    }
    return;
}

static void draw_lat_walls(t_frame *f, t_ivec2 p, t_ivec2 i)
{
    if (i.x > 0 && man.map.cells[i.y * man.map.size.x + (i.x - 1)].symbol != '1')
        draw_sprite(f, &man.sprites[6], p, 0);
    set_ivec2(&p, p.x + man.sprites[6].size.x * 2, p.y);
    if (i.x < man.map.size.x - 1
        && man.map.cells[i.y * man.map.size.x + (i.x + 1)].symbol != '1')
        draw_sprite(f, &man.sprites[6], p, 0);
    return;
}

static void draw_bot_walls(t_frame *f, t_ivec2 p, t_ivec2 i)
{
    char symbol;

    if ((i.y + 1) * man.map.size.x + i.x >= man.map.size.x * man.map.size.y)
        return;
    symbol = man.map.cells[(i.y + 1) * man.map.size.x + i.x].symbol;
    p.y += man.sprites[4].size.y / 2;
    if (i.y < man.map.size.y - 1 && symbol != '1')
        draw_sprite(f, &man.sprites[4], p, 0);
    if (i.y < man.map.size.y - 1 && symbol == '1' && i.x > 0
        && man.map.cells[i.y * man.map.size.x + (i.x - 1)].symbol == '1'
        && man.map.cells[(i.y + 1) * man.map.size.x + (i.x - 1)].symbol != '1')
    {
        draw_sprite(f, &man.sprites[3], p, 0);
        draw_sprite(f, &man.sprites[6], p, 0);
    }
    p.x += man.sprites[3].size.x * 2;
    if (i.y < man.map.size.y - 1 && symbol == '1' && i.x < man.map.size.x - 1
        && man.map.cells[i.y * man.map.size.x + (i.x + 1)].symbol == '1'
        && man.map.cells[(i.y + 1) * man.map.size.x + (i.x + 1)].symbol != '1')
    {
        draw_sprite(f, &man.sprites[3], p, 0);
        draw_sprite(f, &man.sprites[6], p, 0);
    }
    return;
}
