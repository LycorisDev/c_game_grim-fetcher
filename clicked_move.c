#include "grim_fetcher.h"

static t_list	*get_clicked_path(t_win *win);

void	move_along_clicked_path(t_win *win, long dt)
{
	static long	elapsed_time;
	t_ivec2		move;

	if (!win->clicked_path)
		win->clicked_path = get_clicked_path(win);
	if (!win->clicked_path)
	{
		win->click_pos.x = 0;
		win->click_pos.y = 0;
		return ;
	}
	elapsed_time += dt;
	if (!dt)
		elapsed_time += 50;
	if (elapsed_time >= 200)
	{
		elapsed_time -= 200;
		move.x = ((t_cell *)win->clicked_path->content)->pos.x - win->p.pos.x;
		move.y = ((t_cell *)win->clicked_path->content)->pos.y - win->p.pos.y;
		move_player(win, move);
		lst_pop(&win->clicked_path, 0);
	}
	return ;
}

t_ivec2	slime_get_move_to_kill_player(t_cell *slime)
{
	t_ivec2	move;
	int		i;

	i = 0;
	while (i < 4)
	{
		if (slime->neighbors[i]->symbol == 'P')
		{
			set_ivec2(&move, slime->neighbors[i]->pos.x - slime->pos.x,
				slime->neighbors[i]->pos.y - slime->pos.y);
			return (move);
		}
		++i;
	}
	set_ivec2(&move, 0, 0);
	return (move);
}

static t_list	*get_clicked_path(t_win *win)
{
	t_cell	*player_cell;
	t_cell	*cell;
	t_ivec2	origin;

	if (!win->click)
		return (0);
	player_cell = find_cell_by_symbol(win->map.cells, 'P');
	origin.x = (13 / win->zoom - player_cell->pos.x) * 24;
	origin.y = (7 / win->zoom - player_cell->pos.y) * 24;
	win->click_pos.x = win->click_pos.x / win->zoom / 24 - origin.x / 24;
	win->click_pos.y = win->click_pos.y / win->zoom / 24 - origin.y / 24;
	cell = win->map.cells;
	while (cell->symbol)
	{
		if (cell->pos.x == win->click_pos.x && cell->pos.y == win->click_pos.y
			&& (!cell->is_obstacle || cell->symbol == 'e'))
			return (get_path(win, player_cell, cell));
		++cell;
	}
	return (0);
}
