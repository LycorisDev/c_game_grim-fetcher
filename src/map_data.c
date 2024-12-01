#include "grim_fetcher.h"

static int are_symbols_valid(char **map);
static int are_symbols_in_right_amount(char **map);
static int is_map_rectangular_and_closed_off(char **map);
static int is_map_closed_off_by_walls(char **map);

char **get_map_data(char *path)
{
    int  fd;
    char **map;

    fd = get_map_fd(path);
    if (fd < 0)
        return 0;
    map = copy_map(&fd, path);
    close(fd);
    if (!are_symbols_valid(map) || !is_map_rectangular_and_closed_off(map))
    {
        free_map_data(map);
        return 0;
    }
    return map;
}

void free_map_data(char **map)
{
    char **ptr;

    ptr = map;
    while (*map)
    {
        free(*map);
        ++map;
    }
    free(ptr);
    return;
}

static int are_symbols_valid(char **map)
{
    size_t i;
    size_t j;

    if (!map)
        return 0;
    i = 0;
    while (map[i])
    {
        j = 0;
        while (map[i][j])
        {
            if (!strchr("01BCEGP", map[i][j]))
            {
                dprintf(2, "Error: Unknown symbol '%c' (-> \"%s\")\n",
                    map[i][j], "01BCEGP");
                return 0;
            }
            ++j;
        }
        ++i;
    }
    return are_symbols_in_right_amount(map);
}

static int are_symbols_in_right_amount(char **map)
{
    int amount[3];

    amount[0] = 0;
    amount[1] = 0;
    amount[2] = 0;
    while (*map)
    {
        amount[0] += strchr(*map, 'C') != 0;
        amount[1] += strchr(*map, 'E') != 0;
        amount[2] += strchr(*map, 'P') != 0;
        ++map;
    }
    if (amount[0] >= 1 && amount[1] == 1 && amount[2] == 1)
        return 1;
    if (!amount[0])
        dprintf(2, "Error: No collectible found ('C')\n");
    if (!amount[1])
        dprintf(2, "Error: No exit found ('E')\n");
    else if (amount[1] > 1)
        dprintf(2, "Error: More than one exit found ('E')\n");
    if (!amount[2])
        dprintf(2, "Error: No player start found ('P')\n");
    else if (amount[2] > 1)
        dprintf(2, "Error: More than one player start found ('P')\n");
    return 0;
}

static int is_map_rectangular_and_closed_off(char **map)
{
    size_t i;
    size_t len;
    size_t curr_len;

    i = 0;
    len = 0;
    while (map[i])
    {
        curr_len = strlen(map[i]);
        if (!len)
            len = curr_len;
        else if (curr_len && len != curr_len)
        {
            dprintf(2, "Error: Map is not rectangular\n");
            return 0;
        }
        ++i;
    }
    if (!is_map_closed_off_by_walls(map))
    {
        dprintf(2, "Error: Map is not closed_off_by_walls\n");
        return 0;
    }
    return 1;
}

static int is_map_closed_off_by_walls(char **map)
{
    size_t i;
    size_t j;
    size_t width;

    i = 0;
    width = map[0] ? strlen(map[0]) : 0;
    while (map[i])
    {
        if (map[i][0] != '1' || map[i][width - 1] != '1')
            return 0;
        if (i == 0 || !map[i + 1])
        {
            j = 0;
            while (j < width)
            {
                if (map[i][j] != '1')
                    return 0;
                ++j;
            }
        }
        ++i;
    }
    return 1;
}
