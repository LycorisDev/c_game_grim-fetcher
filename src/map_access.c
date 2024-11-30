#include "grim_fetcher.h"

static t_list *find_targets(t_cell *map);

int are_collectibles_and_exit_accessible(void)
{
    t_list *path;
    t_cell *start;
    t_list *target;

    start = find_cell_by_symbol(man.map.cells, 'P');
    target = find_targets(man.map.cells);
    while (target)
    {
        path = get_path(start, target->data);
        if (!path)
        {
            list_clear(&target, 0);
            return 0;
        }
        list_clear(&path, 0);
        list_pop(&target, 0);
    }
    return 1;
}

t_cell *find_cell_by_symbol(t_cell *map, char symbol)
{
    int i;

    i = 0;
    while (map[i].symbol)
    {
        if (map[i].symbol == symbol)
            return &map[i];
        ++i;
    }
    return 0;
}

static t_list *find_targets(t_cell *map)
{
    int    i;
    t_list *list;

    i = 0;
    list = list_new(find_cell_by_symbol(map, 'E'));
    while (map[i].symbol)
    {
        if (map[i].symbol == 'C')
            list_add_front(&list, list_new(&map[i]));
        ++i;
    }
    return list;
}
