#include "grim_fetcher.h"

char		**get_ber_data(int argc, char *path);
void		set_player(t_win *win);
t_spr		*get_spr_by_symbol(t_win *win, char symbol);
int			are_collectibles_and_exit_accessible(t_win *win);

static void	set_map_size_and_name(t_win *win, char **ber, char *path);
static int	create_map_array(t_win *win, char **ber);
static void	set_is_obstacle(t_cell *cell);
static void	set_neighbors(t_ivec2 map_size, t_cell *map_cells);

int	set_map_and_player(t_win *win, int argc, char *path)
{
	char	**ber;

	ber = get_ber_data(argc, path);
	if (!ber)
		return (0);
	set_map_size_and_name(win, ber, path);
	if (!create_map_array(win, ber))
	{
		free_ber_data(ber);
		return (0);
	}
	free_ber_data(ber);
	set_player(win);
	set_neighbors(win->map.size, win->map.cells);
	if (!are_collectibles_and_exit_accessible(win))
	{
		ft_dprintf(2, "Error: The exit or a collectible cannot be reached\n");
		return (0);
	}
	return (1);
}

static void	set_map_size_and_name(t_win *win, char **ber, char *path)
{
	t_ivec2	size;

	set_ivec2(&size, 0, 0);
	while (ber[size.y])
	{
		while (ber[size.y][size.x])
			++size.x;
		++size.y;
	}
	set_ivec2(&win->map.size, size.x, size.y);
	if (!ft_strchr(path, '/', 1))
		win->map.name = ft_strdup(path);
	else
		win->map.name = ft_strdup(ft_strchr(path, '/', 1) + 1);
	if (win->map.name)
	{
		win->map.name[ft_strlen(win->map.name) - 4] = 0;
		if (!win->map.name[0])
		{
			free(win->map.name);
			win->map.name = 0;
		}
	}
	return ;
}

static int	create_map_array(t_win *win, char **ber)
{
	t_ivec2	pos;
	t_cell	*c;
	size_t	len;

	len = (win->map.size.x * win->map.size.y + 1) * sizeof(t_cell);
	win->map.cells = malloc(len);
	if (!win->map.cells)
		return (0);
	ft_bzero(win->map.cells, len);
	pos.y = 0;
	while (pos.y < win->map.size.y)
	{
		pos.x = 0;
		while (pos.x < win->map.size.x)
		{
			c = &win->map.cells[pos.y * win->map.size.x + pos.x];
			c->symbol = ber[pos.y][pos.x];
			c->spr = get_spr_by_symbol(win, c->symbol);
			set_ivec2(&c->pos, pos.x, pos.y);
			set_is_obstacle(c);
			++pos.x;
		}
		++pos.y;
	}
	return (1);
}

static void	set_is_obstacle(t_cell *cell)
{
	if (cell->symbol == '0' || cell->symbol == 'C')
		cell->is_obstacle = 0;
	else
		cell->is_obstacle = 1;
	return ;
}

static void	set_neighbors(t_ivec2 map_size, t_cell *map_cells)
{
	int		x;
	int		y;
	t_cell	*c;

	y = 0;
	while (y < map_size.y)
	{
		x = 0;
		while (x < map_size.x)
		{
			c = &map_cells[y * map_size.x + x];
			if (y > 0)
				c->neighbors[0] = &map_cells[(y - 1) * map_size.x + (x + 0)];
			if (x > 0)
				c->neighbors[1] = &map_cells[(y + 0) * map_size.x + (x - 1)];
			if (y < map_size.y - 1)
				c->neighbors[2] = &map_cells[(y + 1) * map_size.x + (x + 0)];
			if (x < map_size.x - 1)
				c->neighbors[3] = &map_cells[(y + 0) * map_size.x + (x + 1)];
			++x;
		}
		++y;
	}
	return ;
}
