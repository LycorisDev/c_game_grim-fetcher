#include "grim_fetcher.h"

static t_ivec2 get_move(t_cell *cell, int rng);
static void    move_slime(t_cell *cell, t_ivec2 move);

void move_symbol(t_map *map, t_ivec2 start, t_ivec2 end)
{
    char  symbol;
    t_spr *spr;

    symbol = map->cells[start.y * map->size.x + start.x].symbol;
    spr = map->cells[start.y * map->size.x + start.x].spr;
    map->cells[start.y * map->size.x + start.x].symbol = '0';
    map->cells[start.y * map->size.x + start.x].is_obstacle = 0;
    map->cells[start.y * map->size.x + start.x].spr = 0;
    map->cells[end.y * map->size.x + end.x].symbol = symbol;
    map->cells[end.y * map->size.x + end.x].is_obstacle = 1;
    map->cells[end.y * map->size.x + end.x].spr = spr;
    return;
}

void move_slimes(long dt)
{
    static long ms;
    size_t      i;
    size_t      j;
    t_ivec2     move;
    t_cell      *slime;

    ms += dt;
    i = 0;
    slime = 0;
    while (man.map.cells[i].symbol)
    {
        if (man.map.cells[i].symbol == 'B' || man.map.cells[i].symbol == 'G')
        {
            slime = &man.map.cells[i];
            j = 0;
            while (j < 4)
            {
                if (slime->neighbors[j]->symbol == 'P')
                {
                    set_ivec2(&move, slime->neighbors[j]->pos.x - slime->pos.x,
                        slime->neighbors[j]->pos.y - slime->pos.y);
                    move_slime(slime, move);
                }
                ++j;
            }
            if (man.player.steps && ms >= 1000 && j == 4)
                move_slime(slime, get_move(slime, (int)dt));
        }
        ++i;
    }
    if (ms >= 1000)
        ms -= 1000;
    return;
}

static t_ivec2 get_move(t_cell *cell, int rng)
{
    t_ivec2 allowed[4];
    size_t  i;
    size_t  j;
    t_ivec2 move;

    i = 0;
    j = 0;
    while (j < 4)
    {
        if (!cell->neighbors[j]->is_obstacle)
        {
            allowed[i].x = cell->neighbors[j]->pos.x - cell->pos.x;
            allowed[i].y = cell->neighbors[j]->pos.y - cell->pos.y;
            dprintf(2, "allowed[%ld].x = %d / allowed[%ld].y = %d\n", i, allowed[i].x, i, allowed[i].y);
            ++i; 
        }
        ++j;
    }
    if (!i)
        set_ivec2(&move, 0, 0);
    else
    {
        rng = rng_minmax(&rng, 0, i - 1);
        set_ivec2(&move, allowed[rng].x, allowed[rng].y);
    }
    return move;
}

static void move_slime(t_cell *cell, t_ivec2 move)
{
    t_ivec2 target;
    char    symbol;

    set_ivec2(&target, cell->pos.x + move.x, cell->pos.y + move.y);
    symbol = man.map.cells[target.y * man.map.size.x + target.x].symbol;
    if (symbol == 'P')
        man.state = FAILURE;
    else if (symbol == '0')
        move_symbol(&man.map, cell->pos, target);
    return;
}
