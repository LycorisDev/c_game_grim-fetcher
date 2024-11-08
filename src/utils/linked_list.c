#include "grim_fetcher.h"

static int list_is_sorted(t_list *list, int (*f)(void *, void *));

t_list *list_new(void *content)
{
    t_list *new;

    new = malloc(sizeof(t_list));
    if (!new)
        return 0;
    new->content = content;
    new->next = 0;
    return new;
}

void list_add_front(t_list **list, t_list *new)
{
    if (!new)
        return;
    new->next = *list;
    *list = new;
    return;
}

int    list_add_sorted(t_list **list, t_list *new, int (*f)(void *, void *))
{
    t_list *curr;
    t_list *prev;

    if (!list_is_sorted(*list, f))
        return 0;
    curr = *list;
    prev = 0;
    while (curr)
    {
        if (f(curr->content, new->content) <= 0)
            prev = curr;
        else
        {
            new->next = curr;
            if (prev)
                prev->next = new;
            else
                *list = new;
            return 1;
        }
        curr = curr->next;
    }
    prev->next = new;
    return 1;
}

void list_pop(t_list **list, void (*del)(void *))
{
    t_list *first;
    t_list *next;

    first = *list;
    next = first->next;
    if (del)
        del(first->content);
    free(first);
    *list = next;
    return;
}

void list_clear(t_list **list, void (*del)(void *))
{
    t_list *next;

    if (!list)
        return;
    while (*list)
    {
        next = (*list)->next;
        if (del)
            del((*list)->content);
        free(*list);
        *list = next;
    }
    *list = 0;
    return;
}

int list_size(t_list *list)
{
    int size;

    size = 0;
    while (list)
    {
        ++size;
        list = list->next;
    }
    return size;
}

static int list_is_sorted(t_list *list, int (*f)(void *, void *))
{
    if (!list || !f)
        return 0;
    while (list->next)
    {
        if (f(list->content, list->next->content) > 0)
            return 0;
        list = list->next;
    }
    return 1;
}
