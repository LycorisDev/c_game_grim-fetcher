#include "grim_fetcher.h"

void	lst_clear(t_list **lst, void (*del)(void *))
{
	t_list	*next;

	if (!lst)
		return ;
	while (*lst)
	{
		next = (*lst)->next;
		if (del)
			del((*lst)->content);
		free(*lst);
		*lst = next;
	}
	*lst = 0;
	return ;
}

int	lst_size(t_list *lst)
{
	int	size;

	size = 0;
	while (lst)
	{
		++size;
		lst = lst->next;
	}
	return (size);
}
