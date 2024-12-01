#include "grim_fetcher.h"

static t_cell  *find_slime_to_kill_player(t_ivec2 *move);
static t_cell  *get_slime(size_t *i);
static t_ivec2 get_move(int rng);
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
    static long   ms;
    static size_t i;
    t_cell        *slime;
    t_ivec2       move;

    slime = find_slime_to_kill_player(&move);
    if (slime)
    {
        move_slime(slime, move);
        return;
    }
    if (!man.player.steps)
        return;
    slime = get_slime(&i);
    if (!slime)
        return;
    ms += dt;
    if (ms < 750)
        return;
    ms -= 750;
    move = get_move((int)dt);
    move_slime(slime, move);
    return;
}

static t_cell *get_slime(size_t *i)
{
    t_cell *slime;

    slime = 0;
    while (man.map.cells[*i].symbol)
    {
        if (man.map.cells[*i].symbol == 'B' || man.map.cells[*i].symbol == 'G')
        {
            slime = &man.map.cells[*i];
            break;
        }
        ++*i;
    }
    if (!man.map.cells[*i].symbol)
        *i = 0;
    return slime;
}

static t_cell *find_slime_to_kill_player(t_ivec2 *move)
{
    size_t i;
    size_t j;
    t_cell *slime;

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
                    set_ivec2(move, slime->neighbors[j]->pos.x - slime->pos.x,
                        slime->neighbors[j]->pos.y - slime->pos.y);
                    return slime;
                }
                ++j;
            }
        }
        ++i;
    }
    return 0;
}

static t_ivec2 get_move(int rng)
{
    t_ivec2 move;

    rng = rng_minmax(&rng, 1, 4);
    set_ivec2(&move, 0, 0);
    if (rng == 1)
        move.x = -1;
    else if (rng == 2)
        move.x = 1;
    else if (rng == 3)
        move.y = -1;
    else if (rng == 4)
        move.y = 1;
    return move;
}

static void move_slime(t_cell *cell, t_ivec2 move)
{
    t_ivec2 target;
    char    symbol;

    set_ivec2(&target, cell->pos.x + move.x, cell->pos.y + move.y);
    symbol = man.map.cells[target.y * man.map.size.x + target.x].symbol;
    if (symbol == 'P')
        man.game_over = -1;
    else if (symbol == '0')
        move_symbol(&man.map, cell->pos, target);
    return;
}
