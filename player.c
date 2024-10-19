#include "grim_fetcher.h"

#define STAMINA_REGEN 10

static void	orient_player(t_win *win, t_cell *player, t_ivec2 move);
static void	collect_soul(t_win *win, t_ivec2 target);

void	move_player(t_win *win, t_ivec2 move)
{
	t_ivec2	target;
	char	symbol;

	if (win->game_over)
		return ;
	orient_player(win, find_cell_by_symbol(win->map.cells, 'P'), move);
	set_ivec2(&target, win->p.pos.x + move.x, win->p.pos.y + move.y);
	symbol = win->map.cells[target.y * win->map.size.x + target.x].symbol;
	if (symbol == 'e')
	{
		++win->p.steps;
		win->game_over = 1;
	}
	else if (symbol == '0' || symbol == 'C')
	{
		if (symbol == 'C')
			collect_soul(win, target);
		move_symbol(&win->map, win->p.pos, target);
		set_ivec2(&win->p.pos, target.x, target.y);
		++win->p.steps;
	}
	return ;
}

static void	orient_player(t_win *win, t_cell *player, t_ivec2 move)
{
	if ((move.x > 0 && !ft_strcmp(player->spr->id, "dog_idle_down_left"))
		|| (move.y > 0 && !ft_strcmp(player->spr->id, "dog_idle_up_right")))
		player->spr = &win->sprites[13];
	else if ((move.x < 0 && !ft_strcmp(player->spr->id, "dog_idle_down_right"))
		|| (move.y > 0 && !ft_strcmp(player->spr->id, "dog_idle_up_left")))
		player->spr = &win->sprites[14];
	else if ((move.y < 0 && !ft_strcmp(player->spr->id, "dog_idle_down_right"))
		|| (move.x > 0 && !ft_strcmp(player->spr->id, "dog_idle_up_left")))
		player->spr = &win->sprites[15];
	else if ((move.y < 0 && !ft_strcmp(player->spr->id, "dog_idle_down_left"))
		|| (move.x < 0 && !ft_strcmp(player->spr->id, "dog_idle_up_right")))
		player->spr = &win->sprites[16];
	return ;
}

static void	collect_soul(t_win *win, t_ivec2 target)
{
	t_cell	*exit;

	++win->p.collected;
	win->map.cells[target.y * win->map.size.x + target.x].spr = 0;
	win->p.stamina = ft_clamp(win->p.stamina + STAMINA_REGEN, 0, 100);
	if (win->p.collected == win->p.to_collect)
	{
		exit = find_cell_by_symbol(win->map.cells, 'E');
		exit->symbol = 'e';
		exit->spr = get_spr_by_symbol(win, exit->symbol);
	}
	return ;
}
