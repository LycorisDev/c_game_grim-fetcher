#include "grim_fetcher.h"

static void   reset_astar_data(t_map *map, t_cell *start, t_cell *end);
static double heuristic(t_map *map, t_cell *a, t_cell *b);
static void   neighbors(t_map *map, t_list **list, t_cell *curr, t_cell *end);
static int    cmp_global_goal(void *a, void *b);

/* AStar Pathfinding Algorithm */
t_list *get_path(t_win *win, t_cell *start, t_cell *end)
{
    t_cell *curr;
    t_list *list;

    reset_astar_data(&win->map, start, end);
    curr = start;
    list = list_new(start);
    while (list && curr != end)
    {
        while (list && ((t_cell *)(list->content))->is_visited)
            list_pop(&list, 0);
        if (!list)
            break;
        curr = list->content;
        curr->is_visited = 1;
        neighbors(&win->map, &list, curr, end);
    }
    list_clear(&list, 0);
    if (!end->parent)
        return 0;
    while (end != start)
    {
        list_add_front(&list, list_new(end));
        end = end->parent;
    }
    return list;
}

static void reset_astar_data(t_map *map, t_cell *start, t_cell *end)
{
    int i;

    i = 0;
    while (map->cells[i].symbol)
    {
        map->cells[i].is_visited = 0;
        map->cells[i].local_goal = -1;
        map->cells[i].global_goal = -1;
        map->cells[i].parent = 0;
        ++i;
    }
    start->local_goal = 0;
    start->global_goal = heuristic(map, start, end);
    return;
}

static double heuristic(t_map *map, t_cell *a, t_cell *b)
{
    double  base_heuristic;
    double  penalty;
    t_ivec2 step;
    int     x;
    int     y;

    base_heuristic = f_dist_manhattan(a->pos.x, a->pos.y, b->pos.x, b->pos.y);
    penalty = 0;
    set_ivec2(&step, f_clamp(b->pos.x - a->pos.x, -1, 1),
        f_clamp(b->pos.y - a->pos.y, -1, 1));
    x = a->pos.x;
    y = a->pos.y;
    while (x != b->pos.x || y != b->pos.y)
    {
        if (x != b->pos.x)
            x += step.x;
        if (y != b->pos.y)
            y += step.y;
        if (map->cells[y * map->size.x + x].is_obstacle)
            penalty += 1;
    }
    return base_heuristic + penalty;
}

static void neighbors(t_map *map, t_list **list, t_cell *curr, t_cell *end)
{
    int    i;
    t_cell *n;
    double goal;

    i = 0;
    while (i < 4)
    {
        n = curr->neighbors[i];
        if (n && (!n->is_obstacle || n == end) && !n->is_visited)
        {
            goal = curr->local_goal + heuristic(map, curr, n);
            if (goal < n->local_goal || n->local_goal < 0)
            {
                n->parent = curr;
                n->local_goal = goal;
                n->global_goal = n->local_goal + heuristic(map, n, end);
                list_add_sorted(list, list_new(n), cmp_global_goal);
            }
        }
        ++i;
    }
    return;
}

static int cmp_global_goal(void *a, void *b)
{
    t_cell *cell_a;
    t_cell *cell_b;

    cell_a = (t_cell *)a;
    cell_b = (t_cell *)b;
    return cell_a->global_goal - cell_b->global_goal;
}
