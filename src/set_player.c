#include "grim_fetcher.h"

// Used in set_map.c
void	set_player(t_win *win)
{
	int	x;
	int	y;

	ft_bzero(&win->p, sizeof(t_player));
	win->p.health = 100;
	win->p.stamina = 100;
	if (!win->map.cells)
		return ;
	y = 0;
	while (y < win->map.size.y)
	{
		x = 0;
		while (x < win->map.size.x)
		{
			if (win->map.cells[y * win->map.size.x + x].symbol == 'P')
				set_ivec2(&win->p.pos, x, y);
			else if (win->map.cells[y * win->map.size.x + x].symbol == 'C')
				++win->p.to_collect;
			++x;
		}
		++y;
	}
	return ;
}
