#include "grim_fetcher.h"

static t_list *get_clicked_path(void);

void move_along_clicked_path(long dt)
{
    static long elapsed_time;
    t_ivec2     move;

    if (!man.clicked_path)
        man.clicked_path = get_clicked_path();
    if (!man.clicked_path)
    {
        man.click_pos.x = 0;
        man.click_pos.y = 0;
        return;
    }
    elapsed_time += dt;
    if (!dt)
        elapsed_time += 50;
    if (elapsed_time >= 200)
    {
        elapsed_time -= 200;
        move.x = ((t_cell *)man.clicked_path->data)->pos.x - man.player.pos.x;
        move.y = ((t_cell *)man.clicked_path->data)->pos.y - man.player.pos.y;
        move_player(move.x, move.y);
        list_pop(&man.clicked_path, 0);
    }
    return;
}

t_ivec2 slime_get_move_to_kill_player(t_cell *slime)
{
    t_ivec2 move;
    int     i;

    i = 0;
    while (i < 4)
    {
        if (slime->neighbors[i]->symbol == 'P')
        {
            set_ivec2(&move, slime->neighbors[i]->pos.x - slime->pos.x,
                slime->neighbors[i]->pos.y - slime->pos.y);
            return move;
        }
        ++i;
    }
    set_ivec2(&move, 0, 0);
    return move;
}

static t_list *get_clicked_path(void)
{
    t_cell  *player_cell;
    t_cell  *cell;
    t_ivec2 origin;

    if (!man.click)
        return 0;
    player_cell = find_cell_by_symbol(man.map.cells, 'P');
    origin.x = (13 / man.zoom - player_cell->pos.x) * 24;
    origin.y = (7 / man.zoom - player_cell->pos.y) * 24;
    man.click_pos.x = man.click_pos.x / man.zoom / 24 - origin.x / 24;
    man.click_pos.y = man.click_pos.y / man.zoom / 24 - origin.y / 24;
    cell = man.map.cells;
    while (cell->symbol)
    {
        if (cell->pos.x == man.click_pos.x && cell->pos.y == man.click_pos.y
            && (!cell->is_obstacle || cell->symbol == 'e'))
            return get_path(player_cell, cell);
        ++cell;
    }
    return 0;
}
