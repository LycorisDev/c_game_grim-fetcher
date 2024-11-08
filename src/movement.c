#include "grim_fetcher.h"

static t_cell  *get_slime(t_win *win, int *i);
static t_ivec2 get_move(int rng);
static void    move_slime(t_win *win, t_cell *cell, t_ivec2 move);

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

void move_slimes(t_win *win, long dt)
{
    static long ms;
    static int  i;
    t_cell      *slime;
    t_ivec2     move;

    slime = get_slime(win, &i);
    if (!slime)
        return;
    move = slime_get_move_to_kill_player(slime);
    if (!move.x && !move.y)
    {
        ms += dt;
        if (ms < 750)
            return;
        ms -= 750;
        move = get_move((int)dt);
    }
    move_slime(win, slime, move);
    return;
}

static t_cell *get_slime(t_win *win, int *i)
{
    t_cell *slime;

    slime = 0;
    while (win->map.cells[*i].symbol)
    {
        if (win->map.cells[*i].symbol == 'B'
            || win->map.cells[*i].symbol == 'G')
        {
            slime = &win->map.cells[*i];
            break;
        }
        ++*i;
    }
    if (!win->map.cells[*i].symbol)
        *i = 0;
    return slime;
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

static void move_slime(t_win *win, t_cell *cell, t_ivec2 move)
{
    t_ivec2 target;
    char    symbol;

    set_ivec2(&target, cell->pos.x + move.x, cell->pos.y + move.y);
    symbol = win->map.cells[target.y * win->map.size.x + target.x].symbol;
    if (symbol == 'P')
        win->game_over = -1;
    else if (symbol == '0')
        move_symbol(&win->map, cell->pos, target);
    return;
}
