#include "grim_fetcher.h"

static void set_map_size_and_name(char **ber, char *path);
static int  create_map_array(char **ber);
static void set_is_obstacle(t_cell *cell);
static void set_player(void);
static void set_neighbors(t_ivec2 map_size, t_cell *map_cells);

int set_map_and_player(int argc, char *path)
{
    char **ber;

    ber = get_ber_data(argc, path);
    if (!ber)
        return 0;
    set_map_size_and_name(ber, path);
    if (!create_map_array(ber))
    {
        free_ber_data(ber);
        return 0;
    }
    free_ber_data(ber);
    set_player();
    set_neighbors(man.map.size, man.map.cells);
    if (!are_collectibles_and_exit_accessible())
    {
        dprintf(2, "Error: The exit or a collectible cannot be reached\n");
        return 0;
    }
    return 1;
}

static void set_map_size_and_name(char **ber, char *path)
{
    t_ivec2 size;

    set_ivec2(&size, 0, 0);
    while (ber[size.y])
    {
        while (ber[size.y][size.x])
            ++size.x;
        ++size.y;
    }
    set_ivec2(&man.map.size, size.x, size.y);
    if (!strrchr(path, '/'))
        man.map.name = strdup(path);
    else
        man.map.name = strdup(strrchr(path, '/') + 1);
    if (man.map.name)
    {
        man.map.name[strlen(man.map.name) - 4] = 0;
        if (!man.map.name[0])
        {
            free(man.map.name);
            man.map.name = 0;
        }
    }
    return;
}

static int create_map_array(char **ber)
{
    t_ivec2 pos;
    t_cell  *c;
    size_t  len;

    len = (man.map.size.x * man.map.size.y + 1) * sizeof(t_cell);
    man.map.cells = malloc(len);
    if (!man.map.cells)
        return 0;
    bzero(man.map.cells, len);
    pos.y = 0;
    while (pos.y < man.map.size.y)
    {
        pos.x = 0;
        while (pos.x < man.map.size.x)
        {
            c = &man.map.cells[pos.y * man.map.size.x + pos.x];
            c->symbol = ber[pos.y][pos.x];
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

    bzero(&man.player, sizeof(t_player));
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
