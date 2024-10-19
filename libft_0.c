#include "grim_fetcher.h"

int	ft_isspace(int c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
}

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	ft_isupper(int c)
{
	return (c >= 'A' && c <= 'Z');
}

int	ft_islower(int c)
{
	return (c >= 'a' && c <= 'z');
}

size_t	ft_strlen(char *s)
{
	size_t	i;

	i = 0;
	if (s)
	{
		while (s[i])
			++i;
	}
	return (i);
}
