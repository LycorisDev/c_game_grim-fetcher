#include "grim_fetcher.h"

static int	lst_is_sorted(t_list *lst, int (*f)(void *, void *));

t_list	*lst_new(void *content)
{
	t_list	*new;

	new = malloc(sizeof(t_list));
	if (!new)
		return (0);
	new->content = content;
	new->next = 0;
	return (new);
}

void	lst_add_front(t_list **lst, t_list *new)
{
	if (!new)
		return ;
	new->next = *lst;
	*lst = new;
	return ;
}

int	lst_add_sorted(t_list **lst, t_list *new, int (*f)(void *, void *))
{
	t_list	*curr;
	t_list	*prev;

	if (!lst_is_sorted(*lst, f))
		return (0);
	curr = *lst;
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
				*lst = new;
			return (1);
		}
		curr = curr->next;
	}
	prev->next = new;
	return (1);
}

void	lst_pop(t_list **lst, void (*del)(void *))
{
	t_list	*first;
	t_list	*next;

	first = *lst;
	next = first->next;
	if (del)
		del(first->content);
	free(first);
	*lst = next;
	return ;
}

static int	lst_is_sorted(t_list *lst, int (*f)(void *, void *))
{
	if (!lst || !f)
		return (0);
	while (lst->next)
	{
		if (f(lst->content, lst->next->content) > 0)
			return (0);
		lst = lst->next;
	}
	return (1);
}
