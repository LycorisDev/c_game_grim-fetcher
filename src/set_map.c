#include "grim_fetcher.h"

static void set_map_size(char **map);
static int  create_map_array(char **map);
static void set_is_obstacle(t_cell *cell);
static void set_player(void);
static void set_neighbors(t_ivec2 map_size, t_cell *map_cells);

int set_map_and_player(char *path)
{
	char **map;

	map = get_map_data(path);
	if (!map)
		return 0;
	set_map_size(map);
	if (!create_map_array(map))
	{
		free_map_data(map);
		return 0;
	}
	free_map_data(map);
	set_player();
	set_neighbors(man.map.size, man.map.cells);
	if (!are_collectibles_and_exit_accessible())
	{
		fprintf(stderr, "Error: The exit or a collectible cannot be reached\n");
		return 0;
	}
	return 1;
}

static void set_map_size(char **map)
{
	t_ivec2 size;

	set_ivec2(&size, 0, 0);
	while (map[size.y])
	{
		while (map[size.y][size.x])
			++size.x;
		++size.y;
	}
	set_ivec2(&man.map.size, size.x, size.y);
	return;
}

static int create_map_array(char **map)
{
	t_ivec2 pos;
	t_cell  *c;
	size_t  len;

	len = (man.map.size.x * man.map.size.y + 1) * sizeof(t_cell);
	man.map.cells = malloc(len);
	if (!man.map.cells)
		return 0;
	memset(man.map.cells, 0, len);
	pos.y = 0;
	while (pos.y < man.map.size.y)
	{
		pos.x = 0;
		while (pos.x < man.map.size.x)
		{
			c = &man.map.cells[pos.y * man.map.size.x + pos.x];
			c->symbol = map[pos.y][pos.x];
			c->spr = get_spr_by_symbol(c->symbol);
			set_ivec2(&c->pos, pos.x, pos.y);
			set_is_obstacle(c);
			++pos.x;
		}
		++pos.y;
	}
	return 1;
}

static void set_is_obstacle(t_cell *cell)
{
	if (cell->symbol == '0' || cell->symbol == 'C')
		cell->is_obstacle = 0;
	else
		cell->is_obstacle = 1;
	return;
}

static void set_player(void)
{
	int x;
	int y;

	memset(&man.player, 0, sizeof(t_player));
	man.player.health = 100;
	man.player.stamina = 100;
	if (!man.map.cells)
		return;
	y = 0;
	while (y < man.map.size.y)
	{
		x = 0;
		while (x < man.map.size.x)
		{
			if (man.map.cells[y * man.map.size.x + x].symbol == 'P')
				set_ivec2(&man.player.pos, x, y);
			else if (man.map.cells[y * man.map.size.x + x].symbol == 'C')
				++man.player.to_collect;
			++x;
		}
		++y;
	}
	return;
}

static void set_neighbors(t_ivec2 map_size, t_cell *map_cells)
{
	int    x;
	int    y;
	t_cell *c;

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
	return;
}
