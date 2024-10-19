#include "grim_fetcher.h"

static int	get_max(int a, int b);
static int	get_abs(int x);

void	draw_line(t_frame *frame, t_uint color, t_ivec2 p, t_ivec2 dir)
{
	int		steps;
	float	coords_x;
	float	coords_y;
	float	increment_x;
	float	increment_y;

	steps = get_max(get_abs(dir.x), get_abs(dir.y));
	coords_x = p.x;
	coords_y = p.y;
	increment_x = dir.x / (float)steps;
	increment_y = dir.y / (float)steps;
	++steps;
	while (steps-- > 0)
	{
		set_ivec2(&dir, coords_x, coords_y);
		draw_point(frame, color, dir);
		coords_x += increment_x;
		coords_y += increment_y;
	}
	return ;
}

static int	get_max(int a, int b)
{
	if (a > b)
		return (a);
	return (b);
}

static int	get_abs(int x)
{
	if (x < 0)
		return (-x);
	return (x);
}
